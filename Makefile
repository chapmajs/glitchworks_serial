CFLAGS=-g -Wall

posix_test: driver.o posix_serial.o
	cc -o posix_test driver.o posix_serial.o

posix_serial: posix_serial.c glitchworks_serial.h
	cc -c posix_serial.c

driver: driver.c glitchworks_serial.h
	cc -c driver.c

clean:
	rm posix_test *.o

