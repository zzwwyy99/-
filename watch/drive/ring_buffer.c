#include <string.h>

#include "ring_buffer.h"

void ring_buffer_init(ring_buffer *pbuffer)
{
	pbuffer->pRead = pbuffer->pWrite;
}

//写入一个字符
int ring_buffer_write(char c,ring_buffer *pbuffer)
{
	int i = (pbuffer->pWrite + 1 ) % RING_BUFFER_SIZE;

	if(i != pbuffer->pRead){		//环形缓冲区未满
		pbuffer->buffer[pbuffer->pWrite] = c;
		pbuffer->pWrite = i;
	}
	return 0;
}

//读出一个字符
int ring_buffer_read(char *c,ring_buffer *pbuffer)
{	
	if(pbuffer->pRead == pbuffer->pWrite){		//环形缓冲区为空
		return -1;
	}else{
		*c = pbuffer->buffer[pbuffer->pRead];
		pbuffer->pRead = (pbuffer->pRead + 1) % RING_BUFFER_SIZE;
		return 0;
	}
}




