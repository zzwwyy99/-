#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "beep_device.h"

#define BEEP_DEVICE_LIST_MAX	2

//这一层相当于设备驱动框架层

/* 定义一个BeepDevice管理器 */
typedef struct BeepDeviceManager{
	unsigned int num;
	struct BeepDevice *beep_device_list[BEEP_DEVICE_LIST_MAX];
}BeepDeviceManager;

//BeepDevice管理器
static BeepDeviceManager g_tBeepDeviceManager;

/*注册函数*/
void BeepDeviceRegister(struct BeepDevice *ptBeepDevice,char *name)
{
	/* 初始化BeepDevice */
	ptBeepDevice->init(ptBeepDevice);
	ptBeepDevice->name = name;
	printf("注册BeepDevice:%s成功.\r\n",name);
	
	/*  添加BeepDevice到BeepDevice管理器 */
	g_tBeepDeviceManager.beep_device_list[g_tBeepDeviceManager.num++] = ptBeepDevice;
}

/*获得一个设备*/
PBeepDevice __GetBeepDevice(char *name)
{
	PBeepDevice ptBeepDevice;

	for(int i=0;i<g_tBeepDeviceManager.num;i++){
		ptBeepDevice = g_tBeepDeviceManager.beep_device_list[i];
		if(strcmp(ptBeepDevice->name,name) == 0){
            printf("得到设备:%s\r\n",name);
            
			return ptBeepDevice;			
		}
	}
    printf("没有得到设备:%s\r\n",name);
    
	return NULL;
}
