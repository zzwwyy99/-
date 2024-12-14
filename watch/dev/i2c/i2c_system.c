#include "i2c_device.h"
#include "i2c.h"

void ADDI2CDevice(void)
{
	/* 添加I2C设备到I2C设备管理器 */
	AddI2CDeviceI2C1();
}

/*获得一个按键*/
PI2CDevice GetI2CDevice(char *name)
{
	return __GetI2CDevice(name);
}

