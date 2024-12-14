#include <stdio.h>

#include "display_device.h"
#include "oled.h"
#include "display_system.h"

PDisplayDevice g_ptFaultDisplayDev;

void ADDDisplayDevice(void)
{
	/* 添加Display设备到DisplayDevice设备管理器 */
	AddDisplayDeviceOLED();
}

/*获得一个DisplayDevice*/
PDisplayDevice GetDisplayDevice(char *name)
{
	return __GetDisplayDevice(name);
}

//设定一个默认DisplayDevice
PDisplayDevice SetDefaultDisplayDev(void)
{
	/* 添加显示设备到管理器 */
	ADDDisplayDevice();
	
	g_ptFaultDisplayDev = GetDisplayDevice("oled");

	if(g_ptFaultDisplayDev){
		printf("设置默认DisplayDevice：OLED成功.\r\n");
		return g_ptFaultDisplayDev;
	}else{
		printf("设置默认DisplayDevice：OLED失败.\r\n");
		return 0;
	}
}

int iFPS = 0;
//定时刷新
int TimerFlushDisplayDev(void)
{  
	 iFPS++;
	 
     g_ptFaultDisplayDev->flush(g_ptFaultDisplayDev);

	 return 0;
}

//定时器刷新显示设备开始
void TimerFlushDisplayDevControl(int arg)
{  
	__DisplayDevTimerFlushControl(arg);
}

