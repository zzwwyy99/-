#ifndef __KAL_BEEP_H
#define __KAL_BEEP_H

#include "beep_device.h"

void KAL_HardwareBeepDevInit(BeepDevice *ptBeepDevice);
void KAL_BeepDevicePWMCfg(BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse);
void KAL_BeepDevicePWMOn(BeepDevice *ptBeepDevice);
void KAL_BeepDevicePWMOff(BeepDevice *ptBeepDevice);

#endif  /*__KAL_BEEP_H*/


