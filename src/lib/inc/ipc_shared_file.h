#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define IPC_FILE "data.dat"
#define DATA_STRING "Deze data gaan we proberen te printen in blink"

void report_and_exit(const char* msg);

void IPC_sharedFile_send(void);

static void IPC_sharedFile_receive(void);