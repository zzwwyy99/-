#ifndef __CAL_I2C_H
#define __CAL_I2C_H

#include "i2c_device.h"

void CAL_GPIOI2CDevInit(I2CDevice *ptI2CDevice);
void CAL_I2CDMACfg(I2CDevice *ptI2CDevice);
void CAL_I2CDMAEnable(I2CDevice *ptI2CDevice);

#endif  /*__CAL_I2C_H*/
