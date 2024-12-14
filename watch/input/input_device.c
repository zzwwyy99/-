#include <string.h>

#include "input_device.h"

//输入设备管理器
static InputDevice *g_ptInputDeviceManager;

//输入设备注册
void InputDeviceRegister(PInputDevice pInputDevice)
{
	pInputDevice->pNext = g_ptInputDeviceManager;
	g_ptInputDeviceManager = pInputDevice;
}

//从设备管理器获得一个输入设备
PInputDevice __GetInputDevice(char *name)
{
	PInputDevice ptemp;
	ptemp = g_ptInputDeviceManager;
	
	while(ptemp){
		if(strcmp(ptemp->name,name) == 0){
			return ptemp;
		}else{
			ptemp = ptemp->pNext;
		}
	}

	return NULL;
}

/* 输入设备初始化 */
void __InitInputDevice(void)
{
	PInputDevice ptemp;
	ptemp = g_ptInputDeviceManager;
	
	while(ptemp){
        ptemp->DevceInit();  //ButtonDev在ButtonSystem中已经初始化
        ptemp = ptemp->pNext;
	}
}
