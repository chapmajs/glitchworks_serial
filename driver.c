/*
 * A test driver for glitchworks_serial
 */
#include <stdio.h>

#include "glitchworks_serial.h"

int main(int argc, char *argv[]) {
  if (open_port("/dev/ttyUSB0", 9600) == -1) {
    fprintf(stderr, "Failed to open serial port!\n");
    return(-1);
  }
 
  /* write a single char out */
  write_byte('*');

  /* write a NULL-terminated string */
  write_string("Testing string writer...\r\n");
  
  /* write as much as a byte array as specified */
  write_bytes("You'll see A\r\nBut not B\r\n", 14);
  
  printf("\n>>> Now type 11 characters on the serial device!\n");

  /* get one char from the port, block until it's read */
  printf("Got single character: %c\n", read_byte());

  char input_buffer[11];
  
  /* read n bytes from the port into a buffer */
  read_bytes(input_buffer, 10);
  input_buffer[10] = 0x00;
  printf("Got 10 characters: %s\n", input_buffer);
  
  return(0);
}
