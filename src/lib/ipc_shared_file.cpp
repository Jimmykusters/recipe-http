#include "ipc_shared_file.h"

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

void IPC_sharedFile_send(void)
{
	struct flock lock;
	lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
	lock.l_whence = SEEK_SET; /* base for seek offsets */
	lock.l_start = 0;         /* 1st byte in file */
	lock.l_len = 0;           /* 0 here means 'until EOF' */
	lock.l_pid = getpid();    /* process id */

	int fd; /* file descriptor to identify a file within a process */
	if ((fd = open(IPC_FILE, O_RDWR | O_CREAT, 0666)) < 0)  /* -1 signals an error */
	{
		report_and_exit("open failed...");
	}

	if (fcntl(fd, F_SETLK, &lock) < 0) /** F_SETLK doesn't block, F_SETLKW does **/
	{
		report_and_exit("fcntl failed to get lock...");
	}
	else 
	{
		write(fd, DATA_STRING, strlen(DATA_STRING)); /* populate data file */
		fprintf(stderr, "Process %d has written to data file...\n", lock.l_pid);
	}

	/* Now release the lock explicitly. */
	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &lock) < 0)
	{
		report_and_exit("explicit unlocking failed...");
	}

	close(fd); /* close the file: would unlock if needed */
}

static void IPC_sharedFile_receive(void)
{
	struct flock lock;
	lock.l_type = F_WRLCK;    /* read/write (exclusive) lock */
	lock.l_whence = SEEK_SET; /* base for seek offsets */
	lock.l_start = 0;         /* 1st byte in file */
	lock.l_len = 0;           /* 0 here means 'until EOF' */
	lock.l_pid = getpid();    /* process id */
	
	int fd; /* file descriptor to identify a file within a process */
	if ((fd = open(IPC_FILE, O_RDONLY)) < 0)  /* -1 signals an error */
	{
		report_and_exit("open to read failed...");
	}

	/* If the file is write-locked, we can't continue. */
	fcntl(fd, F_GETLK, &lock); /* sets lock.l_type to F_UNLCK if no write lock */
	if (lock.l_type != F_UNLCK)
	{
		report_and_exit("file is still write locked...");
	}

	lock.l_type = F_RDLCK; /* prevents any writing during the reading */
	if (fcntl(fd, F_SETLK, &lock) < 0)
	{
		report_and_exit("can't get a read-only lock...");
	}	

	/* Read the bytes (they happen to be ASCII codes) one at a time. */
	int c; /* buffer for read bytes */
	while (read(fd, &c, 1) > 0)    /* 0 signals EOF */
	{
		write(STDOUT_FILENO, &c, 1); /* write one byte to the standard output */
	}

	/* Release the lock explicitly. */
	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &lock) < 0)
	{
		report_and_exit("explicit unlocking failed...");
	}

  	close(fd); /* close the file: would unlock if needed */
}