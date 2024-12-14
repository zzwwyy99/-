#include <string.h>

#include "input_buffer.h"

//����һ�������¼��Ļ��λ�����
static InputEventBuffer g_tInputEventBuffer;

//д��һ��InputEvent
int PutInputBuffer(InputEvent *ptInputEvent)
{
	int i = (g_tInputEventBuffer.pWrite + 1 ) % BUFFER_SIZE;

	/*����ʽ���*/
	if(!ptInputEvent){
		return -1;
	}
    
	if(i != g_tInputEventBuffer.pRead){		//���λ�����δ��
		g_tInputEventBuffer.buffer[g_tInputEventBuffer.pWrite] = *ptInputEvent;
		g_tInputEventBuffer.pWrite = i;
	}
	return 0;
}

//����һ��InputEvent
int GetInputBuffer(InputEvent *ptInputEvent)
{
	/*����ʽ���*/
	if(!ptInputEvent){
		return -1;
	}
    
	if(g_tInputEventBuffer.pRead == g_tInputEventBuffer.pWrite){		//���λ�����Ϊ��
		return -1;
	}else{
		*ptInputEvent = g_tInputEventBuffer.buffer[g_tInputEventBuffer.pRead];
		g_tInputEventBuffer.pRead = (g_tInputEventBuffer.pRead + 1) % BUFFER_SIZE;
		return 0;
	}
}


