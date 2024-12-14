#include <stdlib.h>
#include <string.h>

#include "led_device.h"

#define LED_DEVICE_LIST_MAX	5

/* ����һ��Led_device������ */
typedef struct LedDeviceManager{
	unsigned int num;
	struct LedDevice *led_device_list[LED_DEVICE_LIST_MAX];
}LedDeviceManager;

//LED�豸������
static LedDeviceManager g_tLedDeviceManager;

/*ע�ắ��*/
void LedDeviceRegister(struct LedDevice *ptLedDevice,char *name)
{
	/* ��ʼ��LedDevice */
	ptLedDevice->init(ptLedDevice);
	ptLedDevice->name = name;
	
	/*  ���LedDevice��LED�豸������ */
	g_tLedDeviceManager.led_device_list[g_tLedDeviceManager.num++] = ptLedDevice;
}

/*���һ��dev*/
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

