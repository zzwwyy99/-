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


/*定义线程控制块*/
struct rt_thread g_tLogicThread;
struct rt_thread g_tMenuThread;
struct rt_thread g_tPlayerThread;
struct rt_thread g_tGameThread;
/*定义线程栈*/
unsigned char LogicThreadStack[512];
unsigned char MenuThreadStack[512];
unsigned char PlayerThreadStack[512];
unsigned char GameThreadStack[512];

extern struct rt_thread *g_ptScheduleThread;

int main(void)
{	
	/* 设备初始化 */
    BoradInit();
    
    /* 调度器初始化 */
    rt_system_scheduler_init();
    
    /*初始化线程*/
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
    
    /* 启动系统调度器 */
    g_ptScheduleThread = &g_tMenuThread;
	rt_system_scheduler_start(&g_tMenuThread);
	
}

