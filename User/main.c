#include "stm32f10x.h"  
#include "stdio.h"

#if 0
#include "usart1_test.h"
#include "input_test.h"
#include "led_test.h"
#include "display_test.h"
#include "text_test.h"
#include "beep_test.h"
#include "decode_test.h"
#include "player_test.h"
#include "graph_test.h"
#include "iterator_test.h"
#include "game_test.h"
#include "menu_test.h"
#include "message_test.h"
#include "logic_control.h"

int main(void)
{    
	usart1_test();
    
    input_test();
    
    text_test();
    
    /* ���������� */
    //decode_test();
   
    /*����������*/
    //player_test();
    
    /* ͼƬ�ƶ����� */
    //graph_test();
    
     /* ���������� */
    //iterator_test();
    
    /* ��Ϸ���� */
    //game_test();
    
    /* �˵����� */
    //menu_test();
    
     /* ��Ϣͨ�Ų��� */
    //message_test();
    
    //ҵ����ϵͳ����
    //logic_test();
    
    //ʱ�Ӳ��� 
    rtc_test();
    printf("over.\r\n");
   
    //�����һ��while(1),��������hard fault�ж�.
    while(1);
}
#endif 


#include <rtthread.h>
#include <rthw.h>

#include "usart_init.h"
#include "button_system.h"
#include "input_system.h"
#include "input_buffer.h"
#include "displaydev_init.h"
#include "beep_system.h"
#include "led_system.h"
#include "player_system.h"
#include "game_system.h"

#include "drive_timer.h"
#include "drive_delay.h"
#include "logic_control.h"
#include "option_manager.h"
#include "rtc_system.h"
#include "dht11_device.h"
#include "rtc_system.h"
#include "rtc_system.h"
#include "message_manager.h"


void BoradInit(void)
{
    //���ڳ�ʼ��
    UsartInit();
    
	//��ʼ�������豸
	AddButtonDevice();
    
	//��ʼ�������豸
    AddInputDevice();
    InitInputDevice();

	//��ʼ��Display�豸
	DisplayDevInit();

	//��ʼ��LedSystem
	AddLedDevice();

	//��ʼ��BeepSystem	
	ADDBeepDevice();

	//��ʼ��ѡ��
	InitMenuOption();

    //Ӳ����ʱ,��ʱ��3����
	DelayDeviceInit();
    
    //DHT11
    DHT11Init();
    
    //Game
    GameThreadInit();
    
    //RTC��ʼ
    RTCInit();
    
    //������ע��
    MenuThreadInit();
    PlayerThreadInit();
    RTCThreadInit();
    
    //������ע��
    LogicThreadInit();
}


/* ������Ϣ���п��ƿ� */
rt_mq_t g_tQueueInput = RT_NULL;
/* �����¼����ƿ� */
rt_event_t g_tBackEvent = RT_NULL;

/* �����ź������ƿ� */
rt_sem_t g_tPlayerSem = RT_NULL;
rt_sem_t g_tGameSem = RT_NULL;

/* �����ź������ƿ� */
rt_mailbox_t g_tMsgCentrerMb = RT_NULL;

//����һ������
int InitInputQueue(void)
{	
	/* �����¼���Ϣ���� */
	g_tQueueInput = rt_mq_create("QueueInput",      /* ��Ϣ�������� */
                                 48,                /* ��Ϣ����󳤶� */
                                 10,                /* ��Ϣ���е�������� */
                                 RT_IPC_FLAG_FIFO); /* ����ģʽ FIFO(0x00)*/
	if (g_tQueueInput != RT_NULL){
		rt_kprintf("��Ϣ���д����ɹ���\n\n");
	}else{
        rt_kprintf("��Ϣ���д���ʧ�ܣ�\n\n");
    }

	/* ����һ���¼� */
	g_tBackEvent = rt_event_create("BackEvent",		/* �¼���־������ */
                        RT_IPC_FLAG_PRIO); 			/* �¼�ģʽ FIFO(0x00)*/
  	if (g_tBackEvent != RT_NULL){
		rt_kprintf("�¼������ɹ���\r\n");
	}else{
        rt_kprintf("�¼�����ʧ�ܣ�\r\n");
    }
	
	/* ����һ���ź��� */
	g_tPlayerSem = rt_sem_create("PlayerSem",	/* �ź������� */
                     1,     					/* �ź�����ʼֵ��Ĭ����һ���ź��� */
                     RT_IPC_FLAG_FIFO);		 	/* �ź���ģʽ FIFO(0x00)*/
  	if (g_tPlayerSem != RT_NULL){
		rt_kprintf("�������ź��������ɹ���\n\n");
	}else{
		rt_kprintf("�������ź�������ʧ�ܣ�\n\n");
	}
	
    /* ����һ���ź��� */
	g_tGameSem = rt_sem_create("GameSem",		/* �ź������� */
                     1,     					/* �ź�����ʼֵ��Ĭ����һ���ź��� */
                     RT_IPC_FLAG_FIFO); 		/* �ź���ģʽ FIFO(0x00)*/
  	if (g_tGameSem != RT_NULL){
		rt_kprintf("��Ϸ�ź��������ɹ���\n\n");
	}else{
		rt_kprintf("��Ϸ�ź�������ʧ�ܣ�\n\n");
	}

	/* ����һ���ź��� */
	g_tMsgCentrerMb = rt_mb_create("MsgCentrerMb",		/* �ź������� */
                     10,     					/* �ź�����ʼֵ��Ĭ����һ���ź��� */
                     RT_IPC_FLAG_FIFO); 		/* �ź���ģʽ FIFO(0x00)*/
  	if (g_tMsgCentrerMb != RT_NULL){
		rt_kprintf("��Ϣ�������䴴���ɹ���\n\n");
	}else{
		rt_kprintf("��Ϣ�������䴴��ʧ�ܣ�\n\n");
	}
	return 0;
}

/*�����߳̿��ƿ�*/
rt_thread_t g_ptLogicThread;
rt_thread_t g_ptMenuThread;
rt_thread_t g_ptPlayerThread;
rt_thread_t g_ptGameThread;
rt_thread_t g_ptMessageCentrerThread;
int main(void)
{	
    /* ��ʼ��������� */
	InitInputQueue();
    
    g_ptLogicThread =                       /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "logic_thread",       /* �߳����� */
                      LogicThreadEntry,     /* �߳���ں��� */
                      RT_NULL,              /* �߳���ں������� */
                      512,                  /* �߳�ջ��С */
                      1,                    /* �̵߳����ȼ� */
                      20);                  /* �߳�ʱ��Ƭ */
    
    /* �����̣߳��������� */
    if (g_ptLogicThread != RT_NULL)
        rt_thread_startup(g_ptLogicThread);
    else
        return -1;
    
    g_ptMenuThread =                        /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "menu_thread",        /* �߳����� */
                      MenuThreadEntry,      /* �߳���ں��� */
                      RT_NULL,              /* �߳���ں������� */
                      512,                  /* �߳�ջ��С */
                      4,                    /* �̵߳����ȼ� */
                      20);                  /* �߳�ʱ��Ƭ */
    
    /* �����̣߳��������� */
    if (g_ptMenuThread != RT_NULL)
        rt_thread_startup(g_ptMenuThread);
    else
        return -1;
    
    //printf("��������");
    g_ptPlayerThread =                     	/* �߳̿��ƿ�ָ�� */
    rt_thread_create( "player_thread",    	/* �߳����� */
                      PlayerThreadEntry,   	/* �߳���ں��� */
                      RT_NULL,             	/* �߳���ں������� */
                      512,                 	/* �߳�ջ��С */
                      6,                   	/* �̵߳����ȼ� */
                      20);                 	/* �߳�ʱ��Ƭ */
    
    /* �����̣߳��������� */
    if (g_ptPlayerThread != RT_NULL)
        rt_thread_startup(g_ptPlayerThread);
    else
        return -1;

	g_ptGameThread =                        /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "game_thread",		/* �߳����� */
                      GameThreadEntry,   	/* �߳���ں��� */
                      RT_NULL,         		/* �߳���ں������� */
                      512,             		/* �߳�ջ��С */
                      6,                   	/* �̵߳����ȼ� */
                      20);                 	/* �߳�ʱ��Ƭ */
    
    /* �����̣߳��������� */
    if (g_ptGameThread != RT_NULL)
        rt_thread_startup(g_ptGameThread);
    else
        return -1;

    //printf("��������");
	g_ptMessageCentrerThread =                        /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "Msg_thread",		/* �߳����� */
                      CoreProcssThreadEntry,   	/* �߳���ں��� */
                      RT_NULL,         		/* �߳���ں������� */
                      512,             		/* �߳�ջ��С */
                      1,                   	/* �̵߳����ȼ� */
                      20);                 	/* �߳�ʱ��Ƭ */
    
    /* �����̣߳��������� */
    if (g_ptMessageCentrerThread != RT_NULL)
        rt_thread_startup(g_ptMessageCentrerThread);
    else
        return -1;
}

