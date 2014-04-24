/*
 * A test driver for glitchworks_serial
 */
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "glitchworks_serial.h"

int main(int argc, char *argv[]) {
  char test[] = "/dev/ttyUSB0";
  char *test_ptr = test;
  printf("Open port result: %d\n", open_port(test_ptr, 9600));
  
  write_byte('W');
  
  char in = read_byte();
  printf("Got character: %c\n", in);
  
  exit(0);
}
