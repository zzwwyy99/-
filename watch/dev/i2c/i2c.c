#include <stm32f10x.h>

#include "i2c_device.h"
#include "kal_i2c.h"

/*I2C1 pin脚宏定义*/
#define	I2C1_SCL		GPIO_Pin_6 
#define	I2C1_SDA		GPIO_Pin_7
#define I2C1_GPIO_PORT	GPIOB
#define I2C1_DMA		DMA1_Channel6
#define I2C1_DMA_PERBASEADDR	((unsigned int)&I2C1->DR)

//初始化I2C1
static void I2CDeviceInit(I2CDevice *ptI2CDevice)
{
	KAL_GPIOI2CDevInit(ptI2CDevice);
}

static void I2CDeviceDMACfg(struct I2CDevice *ptI2CDevice)
{
	KAL_I2CDMACfg(ptI2CDevice);
}

static void I2CDeviceDMAEnable(struct I2CDevice *ptI2CDevice)
{
	
	KAL_I2CDMAEnable(ptI2CDevice);
}

static I2CDevice I2C1_DEV = {
	.iPin_SCL = I2C1_SCL,
	.iPin_SDA = I2C1_SDA,
	.iPort = I2C1_GPIO_PORT,

	.iDMA =  I2C1_DMA,
	.iDMAperBaseAddr = I2C1_DMA_PERBASEADDR,
	
	.init = I2CDeviceInit,
	.dam_cfg = I2CDeviceDMACfg,
	.dam_enable = I2CDeviceDMAEnable
};

void AddI2CDeviceI2C1(void)
{
	I2CDeviceRegister(&I2C1_DEV,"i2c1");
}

