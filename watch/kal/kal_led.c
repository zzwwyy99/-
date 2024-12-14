#include "cal_led.h" 
#include "led_device.h"


void KAL_GPIOLedDevInit(LedDevice *ptLedDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_GPIOLedDevInit(ptLedDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
}


void KAL_GPIOLedDevControl(LedDevice *ptLedDevice,int istatus)
{
	/*对于裸机: 使用CAL*/
	CAL_GPIOLedDevControl(ptLedDevice,istatus);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
}


