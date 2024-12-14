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
    
    /* 蜂鸣器测试 */
    //decode_test();
   
    /*播放器测试*/
    //player_test();
    
    /* 图片移动测试 */
    //graph_test();
    
     /* 迭代器测试 */
    //iterator_test();
    
    /* 游戏测试 */
    //game_test();
    
    /* 菜单测试 */
    //menu_test();
    
     /* 消息通信测试 */
    //message_test();
    
    //业务子系统测试
    //logic_test();
    
    //时钟测试 
    rtc_test();
    printf("over.\r\n");
   
    //必须加一个while(1),否则会进入hard fault中断.
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
    //串口初始化
    UsartInit();
    
	//初始化按键设备
	AddButtonDevice();
    
	//初始化输入设备
    AddInputDevice();
    InitInputDevice();

	//初始化Display设备
	DisplayDevInit();

	//初始化LedSystem
	AddLedDevice();

	//初始化BeepSystem	
	ADDBeepDevice();

	//初始化选项
	InitMenuOption();

    //硬件延时,定时器3配置
	DelayDeviceInit();
    
    //DHT11
    DHT11Init();
    
    //Game
    GameThreadInit();
    
    //RTC初始
    RTCInit();
    
    //发布者注册
    MenuThreadInit();
    PlayerThreadInit();
    RTCThreadInit();
    
    //订阅者注册
    LogicThreadInit();
}


/* 定义消息队列控制块 */
rt_mq_t g_tQueueInput = RT_NULL;
/* 定义事件控制块 */
rt_event_t g_tBackEvent = RT_NULL;

/* 定义信号量控制块 */
rt_sem_t g_tPlayerSem = RT_NULL;
rt_sem_t g_tGameSem = RT_NULL;

/* 定义信号量控制块 */
rt_mailbox_t g_tMsgCentrerMb = RT_NULL;

//创建一个队列
int InitInputQueue(void)
{	
	/* 输入事件消息队列 */
	g_tQueueInput = rt_mq_create("QueueInput",      /* 消息队列名字 */
                                 48,                /* 消息的最大长度 */
                                 10,                /* 消息队列的最大容量 */
                                 RT_IPC_FLAG_FIFO); /* 队列模式 FIFO(0x00)*/
	if (g_tQueueInput != RT_NULL){
		rt_kprintf("消息队列创建成功！\n\n");
	}else{
        rt_kprintf("消息队列创建失败！\n\n");
    }

	/* 创建一个事件 */
	g_tBackEvent = rt_event_create("BackEvent",		/* 事件标志组名字 */
                        RT_IPC_FLAG_PRIO); 			/* 事件模式 FIFO(0x00)*/
  	if (g_tBackEvent != RT_NULL){
		rt_kprintf("事件创建成功！\r\n");
	}else{
        rt_kprintf("事件创建失败！\r\n");
    }
	
	/* 创建一个信号量 */
	g_tPlayerSem = rt_sem_create("PlayerSem",	/* 信号量名字 */
                     1,     					/* 信号量初始值，默认有一个信号量 */
                     RT_IPC_FLAG_FIFO);		 	/* 信号量模式 FIFO(0x00)*/
  	if (g_tPlayerSem != RT_NULL){
		rt_kprintf("播放器信号量创建成功！\n\n");
	}else{
		rt_kprintf("播放器信号量创建失败！\n\n");
	}
	
    /* 创建一个信号量 */
	g_tGameSem = rt_sem_create("GameSem",		/* 信号量名字 */
                     1,     					/* 信号量初始值，默认有一个信号量 */
                     RT_IPC_FLAG_FIFO); 		/* 信号量模式 FIFO(0x00)*/
  	if (g_tGameSem != RT_NULL){
		rt_kprintf("游戏信号量创建成功！\n\n");
	}else{
		rt_kprintf("游戏信号量创建失败！\n\n");
	}

	/* 创建一个信号量 */
	g_tMsgCentrerMb = rt_mb_create("MsgCentrerMb",		/* 信号量名字 */
                     10,     					/* 信号量初始值，默认有一个信号量 */
                     RT_IPC_FLAG_FIFO); 		/* 信号量模式 FIFO(0x00)*/
  	if (g_tMsgCentrerMb != RT_NULL){
		rt_kprintf("消息中心邮箱创建成功！\n\n");
	}else{
		rt_kprintf("消息中心邮箱创建失败！\n\n");
	}
	return 0;
}

/*定义线程控制块*/
rt_thread_t g_ptLogicThread;
rt_thread_t g_ptMenuThread;
rt_thread_t g_ptPlayerThread;
rt_thread_t g_ptGameThread;
rt_thread_t g_ptMessageCentrerThread;
int main(void)
{	
    /* 初始化输入队列 */
	InitInputQueue();
    
    g_ptLogicThread =                       /* 线程控制块指针 */
    rt_thread_create( "logic_thread",       /* 线程名字 */
                      LogicThreadEntry,     /* 线程入口函数 */
                      RT_NULL,              /* 线程入口函数参数 */
                      512,                  /* 线程栈大小 */
                      1,                    /* 线程的优先级 */
                      20);                  /* 线程时间片 */
    
    /* 启动线程，开启调度 */
    if (g_ptLogicThread != RT_NULL)
        rt_thread_startup(g_ptLogicThread);
    else
        return -1;
    
    g_ptMenuThread =                        /* 线程控制块指针 */
    rt_thread_create( "menu_thread",        /* 线程名字 */
                      MenuThreadEntry,      /* 线程入口函数 */
                      RT_NULL,              /* 线程入口函数参数 */
                      512,                  /* 线程栈大小 */
                      4,                    /* 线程的优先级 */
                      20);                  /* 线程时间片 */
    
    /* 启动线程，开启调度 */
    if (g_ptMenuThread != RT_NULL)
        rt_thread_startup(g_ptMenuThread);
    else
        return -1;
    
    //printf("到这里了");
    g_ptPlayerThread =                     	/* 线程控制块指针 */
    rt_thread_create( "player_thread",    	/* 线程名字 */
                      PlayerThreadEntry,   	/* 线程入口函数 */
                      RT_NULL,             	/* 线程入口函数参数 */
                      512,                 	/* 线程栈大小 */
                      6,                   	/* 线程的优先级 */
                      20);                 	/* 线程时间片 */
    
    /* 启动线程，开启调度 */
    if (g_ptPlayerThread != RT_NULL)
        rt_thread_startup(g_ptPlayerThread);
    else
        return -1;

	g_ptGameThread =                        /* 线程控制块指针 */
    rt_thread_create( "game_thread",		/* 线程名字 */
                      GameThreadEntry,   	/* 线程入口函数 */
                      RT_NULL,         		/* 线程入口函数参数 */
                      512,             		/* 线程栈大小 */
                      6,                   	/* 线程的优先级 */
                      20);                 	/* 线程时间片 */
    
    /* 启动线程，开启调度 */
    if (g_ptGameThread != RT_NULL)
        rt_thread_startup(g_ptGameThread);
    else
        return -1;

    //printf("到这里了");
	g_ptMessageCentrerThread =                        /* 线程控制块指针 */
    rt_thread_create( "Msg_thread",		/* 线程名字 */
                      CoreProcssThreadEntry,   	/* 线程入口函数 */
                      RT_NULL,         		/* 线程入口函数参数 */
                      512,             		/* 线程栈大小 */
                      1,                   	/* 线程的优先级 */
                      20);                 	/* 线程时间片 */
    
    /* 启动线程，开启调度 */
    if (g_ptMessageCentrerThread != RT_NULL)
        rt_thread_startup(g_ptMessageCentrerThread);
    else
        return -1;
}

