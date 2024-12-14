#ifndef __CAL_LED_H
#define __CAL_LED_H

#include "led_device.h"

void CAL_GPIOLedDevInit(struct LedDevice *ptLedDevice);
void CAL_GPIOLedDevControl(struct LedDevice *ptLedDevice,int istatus);


#endif  /*__CAL_LED_H*/

