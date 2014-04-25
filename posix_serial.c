/*
 * This file is a part of the Glitch Works Serial library.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * POSIX-compatible serial interface
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

extern int errno;		      // Error from system calls et c.
int portfd;			          // File descriptor for the port
struct termios oldterm;		// Original terminal settings

/* 
 * restore_old_termios -- reset the port to initial conditions
 * 
 * Reset the serial device we're using to the settings saved before
 * initializing the serial device.
 */
int restore_termios () {
  if (tcsetattr(portfd, TCSANOW, &oldterm) == -1) {
    return(errno);
  }

  return(0);
}

/*
 * save_old_termios -- copy the current termios struct for the port
 *
 * Get the current termios struct for the serial device we're using and save
 * it for later restoration.
 */
int save_termios () {
  struct termios t;
  
  if (tcgetattr(portfd, &t) == -1) {
    return(errno);
  }

  memcpy(&oldterm,&t,sizeof(t));
  return(0);
}

/*
 * set_raw -- use POSIX termios to put the port into raw mode
 *
 * Get the termios struct for the serial device we're using and put it in RAW
 * mode. Uses cfmakeraw().
 */
int set_raw () {
  struct termios t;

  if (tcgetattr(portfd, &t) == -1) {
    return(errno);
  }
  
  cfmakeraw(&t);

  if (tcsetattr(portfd, TCSANOW, &t) == -1) {
    return(errno);
  }

  return(0);
}

/*
 * get_posix_serial_speed -- return the POSIX serial speed for an int speed
 */
speed_t get_posix_serial_speed (int speed) {
  switch(speed) {
    case 1200:
      return B1200;
    case 9600:
      return B9600;
    case 19200:
      return B19200;
    default:
      return -1;
  }
}

/*
 * set_speed -- set the port speed
 *
 * Set the port speed based on an integer baudrate specification.
 */
int set_speed (int speed) {
  speed_t posix_speed = get_posix_serial_speed(speed);
  printf("POSIX speed for %d: %d\n", speed, posix_speed);

  if (posix_speed == -1) {
    return(-1);
  }

  struct termios t;
  tcgetattr(portfd, &t);
  cfsetispeed(&t, posix_speed);
  cfsetospeed(&t, posix_speed);
  tcsetattr(portfd, TCSANOW, &t);

  return(0);
}

/*
 * open_port -- open the named port for raw read/write access
 *
 * Open the specified port for raw mode read/write access, effectively 8N1.
 * Return 0 on success, something else on failure.
 *
 * port: string representation of port name
 * speed: integer serial port speed
 */
int open_port (char *port, int speed) {
  printf("Opening port %s ...\n", port);
  
  portfd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

  if (portfd == -1) {
    return -1;
  }

  fcntl(portfd, F_SETFL, 0);

  int termios_result = save_termios();

  if (termios_result != 0) {
    return termios_result;
  }

  int set_raw_result = set_raw();

  if (set_raw_result != 0) {
    return set_raw_result;
  }

  return set_speed(speed);
}

/*
 * write_byte -- write a byte to the port
 *
 * Writes a single byte to the serial port. Port must be opened and initialized
 */
void write_byte (char byte) {
  write(portfd, &byte, 1);
}

/*
 * write_string -- write a string to the port
 *
 * Writes a NULL-terminated string to the serial port.
 */
void write_string (char *string) {
  write(portfd, string, strlen(string));
}

/*
 * write_bytes -- write (length) bytes to the port
 * 
 * Writes (length) bytes to the serial port.
 */
void write_bytes (char *bytes, int length) {
  write(portfd, bytes, length);
}

/*
 * read_byte -- read a byte from the port
 * 
 * Reads a single byte from the serial port. Port must be opened and
 * initalized. Blocks until a byte is available.
 */
char read_byte () {
  struct termios t;
  tcgetattr(portfd, &t);
  t.c_cc[VMIN] = 1;
  tcsetattr(portfd, TCSANOW, &t);
  
  char in;
  read(portfd, &in, 1);
  return(in);
}

/* read_bytes -- read (length) bytes from the port into a buffer
 *
 * Reads (length) bytes into the buffer pointed to by *bytes. Set termios
 * VMIN to prevent timeout on input read. Restore before exiting.
 */
void read_bytes (char *bytes, int length) {
  struct termios t;
  tcgetattr(portfd, &t);
  t.c_cc[VMIN] = length;
  tcsetattr(portfd, TCSANOW, &t);
  
  read(portfd, bytes, length);
}
