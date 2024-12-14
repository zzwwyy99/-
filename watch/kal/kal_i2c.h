#ifndef __KAL_I2C_H
#define __KAL_I2C_H

#include "i2c_device.h"

void KAL_GPIOI2CDevInit(I2CDevice *ptI2CDevice);
void KAL_I2CDMACfg(I2CDevice *ptI2CDevice);
void KAL_I2CDMAEnable(I2CDevice *ptI2CDevice);

#endif  /*__KAL_I2C_H*/
