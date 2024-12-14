#ifndef __KAL_LED_H
#define __KAL_LED_H

#include "led_device.h"

void KAL_GPIOLedDevInit(LedDevice *ptLedDevice);
void KAL_GPIOLedDevControl(LedDevice *ptLedDevice,int istatus);

#endif  /*__KAL_LED_H*/
