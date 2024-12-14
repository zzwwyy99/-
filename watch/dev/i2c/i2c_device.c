#include <stdlib.h>
#include <string.h>

#include "i2c_device.h"

#define I2C_DEVICE_LIST_MAX	2

/* ����һ��Led_device������ */
typedef struct I2CDeviceManager{
	unsigned int num;
	struct I2CDevice *i2c_device_list[I2C_DEVICE_LIST_MAX];
}I2CDeviceManager;

//LED�豸������
static I2CDeviceManager g_tI2CDeviceManager;

/*ע�ắ��*/
void I2CDeviceRegister(struct I2CDevice *ptI2CDevice,char *name)
{
	/* ��ʼ��LedDevice */
	ptI2CDevice->init(ptI2CDevice);
	ptI2CDevice->name = name;
//	ptI2CDevice->iDMAmemBaseAddr = membaseaddr;
//	ptI2CDevice->iBufferSize = bufferszie;
	
	/*  ���LedDevice��LED�豸������ */
	g_tI2CDeviceManager.i2c_device_list[g_tI2CDeviceManager.num++] = ptI2CDevice;
}

/*���һ������*/
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

