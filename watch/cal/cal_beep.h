#ifndef __CAL_BEEP_H
#define __CAL_BEEP_H

#include "beep_device.h"

void CAL_HardwareBeepDevInit(BeepDevice *ptBeepDevice);
void CAL_BeepDevicePWMCfg(BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse);
void CAL_BeepDevicePWMOn(BeepDevice *ptBeepDevice);
void CAL_BeepDevicePWMOff(BeepDevice *ptBeepDevice);


#endif  /*__CAL_BEEP_H*/
