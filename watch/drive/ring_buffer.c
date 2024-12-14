#include <string.h>

#include "ring_buffer.h"

void ring_buffer_init(ring_buffer *pbuffer)
{
	pbuffer->pRead = pbuffer->pWrite;
}

//д��һ���ַ�
int ring_buffer_write(char c,ring_buffer *pbuffer)
{
	int i = (pbuffer->pWrite + 1 ) % RING_BUFFER_SIZE;

	if(i != pbuffer->pRead){		//���λ�����δ��
		pbuffer->buffer[pbuffer->pWrite] = c;
		pbuffer->pWrite = i;
	}
	return 0;
}

//����һ���ַ�
int ring_buffer_read(char *c,ring_buffer *pbuffer)
{	
	if(pbuffer->pRead == pbuffer->pWrite){		//���λ�����Ϊ��
		return -1;
	}else{
		*c = pbuffer->buffer[pbuffer->pRead];
		pbuffer->pRead = (pbuffer->pRead + 1) % RING_BUFFER_SIZE;
		return 0;
	}
}




