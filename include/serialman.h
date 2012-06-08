#ifndef _SERIALMAN_H
#define _SERIALMAN_H

#define TTYS0 3
#define TTYS1 4
#define TTYS2 5
#define TTYS3 6

int serialman_write(int serialfd, char * data, int size);
int serialman_read(int serialfd, char * data, int size);

void serialman_init();

#endif
