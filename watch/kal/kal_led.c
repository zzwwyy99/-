#include "cal_led.h" 
#include "led_device.h"


void KAL_GPIOLedDevInit(LedDevice *ptLedDevice)
{
	/*�������: ʹ��CAL*/
	CAL_GPIOLedDevInit(ptLedDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
}


void KAL_GPIOLedDevControl(LedDevice *ptLedDevice,int istatus)
{
	/*�������: ʹ��CAL*/
	CAL_GPIOLedDevControl(ptLedDevice,istatus);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
}


