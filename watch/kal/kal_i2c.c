#include "cal_i2c.h" 
#include "i2c_device.h"

void KAL_GPIOI2CDevInit(I2CDevice *ptI2CDevice)
{
	/*�������: ʹ��CAL*/
	CAL_GPIOI2CDevInit(ptI2CDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
}

void KAL_I2CDMACfg(I2CDevice *ptI2CDevice)
{
	/*�������: ʹ��CAL*/
	CAL_I2CDMACfg(ptI2CDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
}

void KAL_I2CDMAEnable(I2CDevice *ptI2CDevice)
{
	/*�������: ʹ��CAL*/
	CAL_I2CDMAEnable(ptI2CDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
}

