#include "ipc_shared_memory.h"

void IPC_sharedMemory_send(void)
{
    int fd = shm_open( BackingFile,      /* name from smem.h */
                       O_RDWR | O_CREAT, /* read/write, create if needed */
                       AccessPerms);     /* access permissions (0644) */

    if (fd < 0) 
    {
        report_and_exit("Can't open shared mem segment...");
    }

    ftruncate(fd, ByteSize); /* get the bytes */

    caddr_t memptr = static_cast<caddr_t>(mmap((caddr_t)0,       /* let system pick where to put segment */
                          ByteSize,   /* how many bytes */
                          PROT_READ | PROT_WRITE, /* access protections */
                          MAP_SHARED, /* mapping visible to other processes */
                          fd,         /* file descriptor */
                          0));         /* offset: start at 1st byte */
    if ((caddr_t) -1  == memptr) 
    {
        report_and_exit("Can't get segment...");
    }

    fprintf(stderr, "shared mem address: %p [0..%d]\n", memptr, ByteSize - 1);
    fprintf(stderr, "backing file:       /dev/shm%s\n", BackingFile );

    /* semahore code to lock the shared mem */
    sem_t* semptr = sem_open(SemaphoreName, /* name */
                O_CREAT,       /* create the semaphore */
                AccessPerms,   /* protection perms */
                0);            /* initial value */
    if (semptr == (void*) -1) 
    {
        report_and_exit("sem_open");
    }

    strcpy(memptr, MemContents); /* copy some ASCII bytes to the segment */

    /* increment the semaphore so that memreader can read */
    if (sem_post(semptr) < 0) 
    {
        report_and_exit("sem_post");
    }

    sleep(12); /* give reader a chance */

    /* clean up */
    munmap(memptr, ByteSize); /* unmap the storage */
    close(fd);
    sem_close(semptr);
    shm_unlink(BackingFile); /* unlink from the backing file */
}

void IPC_sharedMemory_receive(void)
{
    int fd = shm_open(BackingFile, O_RDWR, AccessPerms);  /* empty to begin */
    if (fd < 0) 
    {
        report_and_exit("Can't get file descriptor...");
    }

    /* get a pointer to memory */
    caddr_t memptr = static_cast<caddr_t>(mmap((caddr_t)0,       /* let system pick where to put segment */
                          ByteSize,   /* how many bytes */
                          PROT_READ | PROT_WRITE, /* access protections */
                          MAP_SHARED, /* mapping visible to other processes */
                          fd,         /* file descriptor */
                          0));         /* offset: start at 1st byte */
    if ((caddr_t) -1 == memptr) 
    {
        report_and_exit("Can't access segment...");
    }

    /* create a semaphore for mutual exclusion */
    sem_t* semptr = sem_open(SemaphoreName, /* name */
                             O_CREAT,       /* create the semaphore */
                             AccessPerms,   /* protection perms */
                             0);            /* initial value */
    if (semptr == (void*) -1) 
    {
        report_and_exit("sem_open");
    }

    /* use semaphore as a mutex (lock) by waiting for writer to increment it */
    if (!sem_wait(semptr)) { /* wait until semaphore != 0 */
        int i;
        for (i = 0; i < strlen(MemContents); i++)
        {
            write(STDOUT_FILENO, memptr + i, 1); /* one byte at a time */
        }
        
        sem_post(semptr);
    }

    /* cleanup */
    munmap(memptr, ByteSize);
    close(fd);
    sem_close(semptr);
    unlink(BackingFile);
}