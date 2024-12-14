#include <stm32f10x.h>

#include "drive_i2c.h"
#include "i2c_device.h"

void CAL_GPIOI2CDevInit(I2CDevice *ptI2CDevice)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_GPIOI2CDevConfig(ptI2CDevice->iPort,ptI2CDevice->iPin_SCL,ptI2CDevice->iPin_SDA);

	/* ��������оƬ */
}

void CAL_I2CDMACfg(I2CDevice *ptI2CDevice)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_I2CDMACfg((DMA_Channel_TypeDef*)ptI2CDevice->iDMA,ptI2CDevice->iDMAperBaseAddr,ptI2CDevice->iDMAmemBaseAddr,ptI2CDevice->iBufferSize);

	/* ��������оƬ */
}

void CAL_I2CDMAEnable(I2CDevice *ptI2CDevice)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_I2CDMAEnable((DMA_Channel_TypeDef*)ptI2CDevice->iDMA);

	/* ��������оƬ */
}



