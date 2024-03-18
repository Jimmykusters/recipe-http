#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "app_utils.h"

#define IPC_FILE "data.dat"
#define DATA_STRING "Deze data gaan we proberen te printen in blink"

void IPC_sharedFile_send(void);
void IPC_sharedFile_receive(void);