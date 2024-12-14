#ifndef __DRIVE_I2C_H
#define __DRIVE_I2C_H

#include <stm32f10x.h>

void STD_GPIOI2CDevConfig(void *port,int pin1,int pin2);
void STD_I2CDMACfg(void* DMA_CHx,unsigned int peripheralBaseAddr,unsigned int memoryBaseAddr,unsigned int bufferSize);
void STD_I2CDMAEnable(void* DMA_CHx);


#endif  /*__DRIVE_I2C_H*/

