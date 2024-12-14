#include <string.h>
#include "rtthread.h"

#include "input_buffer.h"
#include "stdio.h"

#if 0
//定义一个输入事件的环形缓冲区
static InputEventBuffer g_tInputEventBuffer;
#endif

extern rt_mq_t g_tQueueInput;

//写入一个InputEvent
int PutInputBuffer(InputEvent *ptInputEvent)
{
#if 0
	int i = (g_tInputEventBuffer.pWrite + 1 ) % BUFFER_SIZE;

	/*防御式编程*/
	if(!ptInputEvent){
		return -1;
	}
    
	if(i != g_tInputEventBuffer.pRead){		//环形缓冲区未满
		g_tInputEventBuffer.buffer[g_tInputEventBuffer.pWrite] = *ptInputEvent;
		g_tInputEventBuffer.pWrite = i;
	}
#else 
	rt_err_t uwRet = RT_EOK;

	/* 写队列 */
	/* 将数据写入（发送）到队列中，等待时间为 0  */
    uwRet = rt_mq_send(	g_tQueueInput,	        /* 写入（发送）队列的ID(句柄) */
				        ptInputEvent,			/* 写入（发送）的数据 */
				        sizeof(InputEvent));    /* 数据的长度 */
	if(RT_EOK != uwRet){
   		rt_kprintf("数据不能发送到消息队列!错误代码: %lx\n",uwRet);
    }

	return 0;
#endif
}

//读出一个InputEvent
int GetInputBuffer(InputEvent *ptInputEvent)
{
#if 0
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
#else 
	/* 读队列 */
	rt_err_t uwRet = RT_EOK;	
	/* 任务都是一个无限循环，不能返回 */
	
	/* 队列读取（接收），等待时间为一直等待 */
	uwRet = rt_mq_recv(g_tQueueInput,	        /* 读取（接收）队列的ID(句柄) */
                        ptInputEvent,			/* 读取（接收）的数据保存位置 */
                        sizeof(InputEvent),		/* 读取（接收）的数据的长度 */
                        RT_WAITING_FOREVER); 	/* 等待时间：一直等 */
	if(RT_EOK == uwRet){
// 		printf("按键产生输入事件----------\r\n");
//  	printf("按键类型:%d\r\n",ptInputEvent->etype);
//		printf("按键值:%d\r\n",ptInputEvent->iKey);
//		printf("按键事件:%d\r\n",ptInputEvent->iPressure);
	}else{
		rt_kprintf("数据接收出错,错误代码: 0x%lx\n",uwRet);
	}

	return 0;
#endif
}


