#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "display_device.h"

#define DISPLAY_DEVICE_LIST_MAX	2

/* 定义一个DisplayDevice管理器 */
typedef struct DisplayDeviceManager{
	unsigned int num;

	//这里和buttonDev一样,都需要用一个定时器刷新,所以就共用一个了,如果用其他定时器,需要先配置定时器
	//void (*DisplayDevTimerInit)(void);  /* 定时刷新用到的定时器 */

	//开启定时器定时刷新
	void (*TimerFlushControl)(int arg); 
	
	struct DisplayDevice *display_device_list[DISPLAY_DEVICE_LIST_MAX];
}DisplayDeviceManager;

int flush_start = 0;
void DisplayDevTimerFlushControl(int arg)
{
    int cmd = arg;
    
	//帧率测试
    if(cmd == TIMER_FLUSH_CMD_ENABLE){
        //printf("定时器刷新屏幕开启.\r\n");
        flush_start = 1;	//一个标志位,或者信号量
    }else{
        //printf("定时器刷新屏幕关闭.\r\n");
        flush_start = 0;	//一个标志位,或者信号量
    }    
}

//DisplayDevice管理器
static DisplayDeviceManager g_tDisplayDeviceManager ={
	.num = 0,
	.TimerFlushControl = DisplayDevTimerFlushControl
};

void __DisplayDevTimerFlushControl(int arg)
{
	g_tDisplayDeviceManager.TimerFlushControl(arg);
}

/*注册函数*/
void DisplayDeviceRegister(struct DisplayDevice *ptDisplayDevice,char *name)
{
	/* 初始化DisplayDevice */
	ptDisplayDevice->init(ptDisplayDevice);
	ptDisplayDevice->name = name;
	
	/*  添加DisplayDevice到DisplayDevice设备管理器 */
	g_tDisplayDeviceManager.display_device_list[g_tDisplayDeviceManager.num++] = ptDisplayDevice;
}

/*获得dev*/
PDisplayDevice __GetDisplayDevice(char *name)
{
	PDisplayDevice ptDisplayDevice;

	for(int i=0;i<g_tDisplayDeviceManager.num;i++){
		ptDisplayDevice = g_tDisplayDeviceManager.display_device_list[i];
		if(strcmp(ptDisplayDevice->name,name) == 0){
			return ptDisplayDevice;			
		}
	}

	return NULL;
}




