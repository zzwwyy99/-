#include <string.h>

#include "input_buffer.h"

//定义一个输入事件的环形缓冲区
static InputEventBuffer g_tInputEventBuffer;

//写入一个InputEvent
int PutInputBuffer(InputEvent *ptInputEvent)
{
	int i = (g_tInputEventBuffer.pWrite + 1 ) % BUFFER_SIZE;

	/*防御式编程*/
	if(!ptInputEvent){
		return -1;
	}
    
	if(i != g_tInputEventBuffer.pRead){		//环形缓冲区未满
		g_tInputEventBuffer.buffer[g_tInputEventBuffer.pWrite] = *ptInputEvent;
		g_tInputEventBuffer.pWrite = i;
	}
	return 0;
}

//读出一个InputEvent
int GetInputBuffer(InputEvent *ptInputEvent)
{
	/*防御式编程*/
	if(!ptInputEvent){
		return -1;
	}
    
	if(g_tInputEventBuffer.pRead == g_tInputEventBuffer.pWrite){		//环形缓冲区为空
		return -1;
	}else{
		*ptInputEvent = g_tInputEventBuffer.buffer[g_tInputEventBuffer.pRead];
		g_tInputEventBuffer.pRead = (g_tInputEventBuffer.pRead + 1) % BUFFER_SIZE;
		return 0;
	}
}


