#include <stdlib.h>
#include <string.h>

#include "led_device.h"

#define LED_DEVICE_LIST_MAX	5

/* 定义一个Led_device管理器 */
typedef struct LedDeviceManager{
	unsigned int num;
	struct LedDevice *led_device_list[LED_DEVICE_LIST_MAX];
}LedDeviceManager;

//LED设备管理器
static LedDeviceManager g_tLedDeviceManager;

/*注册函数*/
void LedDeviceRegister(struct LedDevice *ptLedDevice,char *name)
{
	/* 初始化LedDevice */
	ptLedDevice->init(ptLedDevice);
	ptLedDevice->name = name;
	
	/*  添加LedDevice到LED设备管理器 */
	g_tLedDeviceManager.led_device_list[g_tLedDeviceManager.num++] = ptLedDevice;
}

/*获得一个dev*/
PLedDevice __GetLedDevice(char *name)
{
	PLedDevice ptLedDevice;

	for(int i=0;i<g_tLedDeviceManager.num;i++){
		ptLedDevice = g_tLedDeviceManager.led_device_list[i];
		if(strcmp(ptLedDevice->name,name) == 0){
			return ptLedDevice;			
		}
	}

	return NULL;
}

