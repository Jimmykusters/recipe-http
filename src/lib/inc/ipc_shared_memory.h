/** Compilation: gcc -o memwriter memwriter.c -lrt -lpthread **/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include "app_utils.h"

#define ByteSize 512
#define BackingFile "/shMemEx"
#define AccessPerms 0644
#define SemaphoreName "mysemaphore"
#define MemContents "This is the way the world ends...\n"

void IPC_sharedMemory_send(void);
void IPC_sharedMemory_receive(void);