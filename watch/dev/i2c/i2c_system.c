#include "i2c_device.h"
#include "i2c.h"

void ADDI2CDevice(void)
{
	/* ���I2C�豸��I2C�豸������ */
	AddI2CDeviceI2C1();
}

/*���һ������*/
PI2CDevice GetI2CDevice(char *name)
{
	return __GetI2CDevice(name);
}

