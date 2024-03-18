#include "app_utils.h"

void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1);
}