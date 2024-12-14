#ifndef __DRIVE_PWM_H
#define __DRIVE_PWM_H

void STD_GPIOBeepDevConfig(void *port,int pin);
void STD_BeepDevicePWMCfg(void *timer,unsigned int period,unsigned int pulse);
void STD_BeepDevicePWMOff(void *timer);

#endif  /*__DRIVE_PWM_H*/

