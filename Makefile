CFLAGS=-g -Wall

posix_test: driver.o posix_serial.o
	cc -o posix_test driver.o posix_serial.o

posix_serial: posix_serial.o
	cc -c posix_serial.c

driver: driver.o
	cc -c driver.c

clean:
	rm posix_test *.o

