#include "drive_gpio.h"
#include "led_device.h"

void CAL_GPIOLedDevInit(LedDevice *ptLedDevice)
{
	/*STD��׼�⣯HAL���оƬ: ʹ��STD_GPIOButtonConfig*/
	STD_GPIOLedDevConfig(ptLedDevice->iPort,ptLedDevice->iPin);

	/*����оƬ*/
}

void CAL_GPIOLedDevControl(LedDevice *ptLedDevice,int istatus)
{
	/*STD��׼�⣯HAL���оƬ: ʹ��STD_GPIOButtonConfig*/
	STD_GPIOLedDevControl(ptLedDevice->iPort,ptLedDevice->iPin,istatus);

	/*����оƬ*/
}


