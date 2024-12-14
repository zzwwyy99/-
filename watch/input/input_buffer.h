#ifndef _INPUT_BUFFER_H
#define _INPUT_BUFFER_H

#include "input_device.h"

#define BUFFER_SIZE 10

typedef struct InputEventBuffer{
	InputEvent buffer[BUFFER_SIZE];
	
	volatile unsigned int pWrite;
	volatile unsigned int pRead;
}InputEventBuffer;

int PutInputBuffer(InputEvent *ptInputEvent);
int GetInputBuffer(InputEvent *ptInputEvent);

#endif /*_INPUT_BUFFER_H*/

