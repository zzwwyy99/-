#include <stdlib.h>
#include <string.h>

#include "i2c_device.h"

#define I2C_DEVICE_LIST_MAX	2

/* 定义一个Led_device管理器 */
typedef struct I2CDeviceManager{
	unsigned int num;
	struct I2CDevice *i2c_device_list[I2C_DEVICE_LIST_MAX];
}I2CDeviceManager;

//LED设备管理器
static I2CDeviceManager g_tI2CDeviceManager;

/*注册函数*/
void I2CDeviceRegister(struct I2CDevice *ptI2CDevice,char *name)
{
	/* 初始化LedDevice */
	ptI2CDevice->init(ptI2CDevice);
	ptI2CDevice->name = name;
//	ptI2CDevice->iDMAmemBaseAddr = membaseaddr;
//	ptI2CDevice->iBufferSize = bufferszie;
	
	/*  添加LedDevice到LED设备管理器 */
	g_tI2CDeviceManager.i2c_device_list[g_tI2CDeviceManager.num++] = ptI2CDevice;
}

/*获得一个按键*/
PI2CDevice __GetI2CDevice(char *name)
{
	PI2CDevice ptI2CDevice;

	for(int i=0;i<g_tI2CDeviceManager.num;i++){
		ptI2CDevice = g_tI2CDeviceManager.i2c_device_list[i];
		if(strcmp(ptI2CDevice->name,name) == 0){
			return ptI2CDevice;			
		}
	}

	return NULL;
}

