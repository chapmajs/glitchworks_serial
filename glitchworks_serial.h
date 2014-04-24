/* A standard serial API for POSIX systems and MS-DOS */

int open_port(char *port, int speed);
int close_port(char **port);
void write_byte(char byte);
