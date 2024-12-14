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

#include <rthw.h>

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


/*�����߳̿��ƿ�*/
struct rt_thread g_tLogicThread;
struct rt_thread g_tMenuThread;
struct rt_thread g_tPlayerThread;
struct rt_thread g_tGameThread;
/*�����߳�ջ*/
unsigned char LogicThreadStack[512];
unsigned char MenuThreadStack[512];
unsigned char PlayerThreadStack[512];
unsigned char GameThreadStack[512];

extern struct rt_thread *g_ptScheduleThread;

int main(void)
{	
	/* �豸��ʼ�� */
    BoradInit();
    
    /* ��������ʼ�� */
    rt_system_scheduler_init();
    
    /*��ʼ���߳�*/
    rt_thread_init(&g_tLogicThread,
   					"logic_thread",
                    LogicThreadEntry,
                    RT_NULL,
                    LogicThreadStack,
                    512);

   rt_thread_init(&g_tMenuThread,
   				"menu_thread",
                MenuThreadEntry,
                RT_NULL,
                MenuThreadStack,
                512);
   
   rt_thread_init(&g_tPlayerThread,
   				"player_thread",
                PlayerThreadEntry,
                RT_NULL,
                PlayerThreadStack,
                512);
   
   rt_thread_init(&g_tGameThread,
   				"game_thread",
                GameThreadEntry,
                RT_NULL,
                GameThreadStack,
                512);
    
    /* ����ϵͳ������ */
    g_ptScheduleThread = &g_tMenuThread;
	rt_system_scheduler_start(&g_tMenuThread);
	
}

