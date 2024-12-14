#include "cal_i2c.h" 
#include "i2c_device.h"

void KAL_GPIOI2CDevInit(I2CDevice *ptI2CDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_GPIOI2CDevInit(ptI2CDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
}

void KAL_I2CDMACfg(I2CDevice *ptI2CDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_I2CDMACfg(ptI2CDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
}

void KAL_I2CDMAEnable(I2CDevice *ptI2CDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_I2CDMAEnable(ptI2CDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
}

