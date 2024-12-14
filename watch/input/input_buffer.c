#include <string.h>
#include "rtthread.h"

#include "input_buffer.h"
#include "stdio.h"

#if 0
//����һ�������¼��Ļ��λ�����
static InputEventBuffer g_tInputEventBuffer;
#endif

extern rt_mq_t g_tQueueInput;

//д��һ��InputEvent
int PutInputBuffer(InputEvent *ptInputEvent)
{
#if 0
	int i = (g_tInputEventBuffer.pWrite + 1 ) % BUFFER_SIZE;

	/*����ʽ���*/
	if(!ptInputEvent){
		return -1;
	}
    
	if(i != g_tInputEventBuffer.pRead){		//���λ�����δ��
		g_tInputEventBuffer.buffer[g_tInputEventBuffer.pWrite] = *ptInputEvent;
		g_tInputEventBuffer.pWrite = i;
	}
#else 
	rt_err_t uwRet = RT_EOK;

	/* д���� */
	/* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
    uwRet = rt_mq_send(	g_tQueueInput,	        /* д�루���ͣ����е�ID(���) */
				        ptInputEvent,			/* д�루���ͣ������� */
				        sizeof(InputEvent));    /* ���ݵĳ��� */
	if(RT_EOK != uwRet){
   		rt_kprintf("���ݲ��ܷ��͵���Ϣ����!�������: %lx\n",uwRet);
    }

	return 0;
#endif
}

//����һ��InputEvent
int GetInputBuffer(InputEvent *ptInputEvent)
{
#if 0
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
#else 
	/* ������ */
	rt_err_t uwRet = RT_EOK;	
	/* ������һ������ѭ�������ܷ��� */
	
	/* ���ж�ȡ�����գ����ȴ�ʱ��Ϊһֱ�ȴ� */
	uwRet = rt_mq_recv(g_tQueueInput,	        /* ��ȡ�����գ����е�ID(���) */
                        ptInputEvent,			/* ��ȡ�����գ������ݱ���λ�� */
                        sizeof(InputEvent),		/* ��ȡ�����գ������ݵĳ��� */
                        RT_WAITING_FOREVER); 	/* �ȴ�ʱ�䣺һֱ�� */
	if(RT_EOK == uwRet){
// 		printf("�������������¼�----------\r\n");
//  	printf("��������:%d\r\n",ptInputEvent->etype);
//		printf("����ֵ:%d\r\n",ptInputEvent->iKey);
//		printf("�����¼�:%d\r\n",ptInputEvent->iPressure);
	}else{
		rt_kprintf("���ݽ��ճ���,�������: 0x%lx\n",uwRet);
	}

	return 0;
#endif
}


