


#ifndef _SERIALMAN_H
#define _SERIALMAN_H

int serialman_write(int serialfd, char * data, int size);
int serialman_read(int serialfd, char * data, int size);

void serialman_init();

#endif
