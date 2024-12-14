#ifndef _INPUT_BUFFER_H
#define _INPUT_BUFFER_H

#include "input_device.h"

#define RING_BUFFER_SIZE 1024

typedef struct ring_buffer{
	unsigned char buffer[RING_BUFFER_SIZE];
	
	volatile unsigned int pWrite;
	volatile unsigned int pRead;
}ring_buffer;

void ring_buffer_init(ring_buffer *pbuffer);
int ring_buffer_write(char c,ring_buffer *pbuffer);
int ring_buffer_read(char *c,ring_buffer *pbuffer);

#endif /*_INPUT_BUFFER_H*/


