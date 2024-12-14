#ifndef __DRIVE_BUTTON_H
#define __DRIVE_BUTTON_H

void STD_GPIOButtonConfig(int button_pin);
void STD_GPIOLedDevConfig(void  *port,int pin);
void STD_GPIOLedDevControl(void *port,int pin,int istatus);

void STD_DHT11GpioConfigure(void *port,int pin);
#endif  /*__DRIVE_BUTTON_H*/

