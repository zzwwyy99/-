#include "drive_gpio.h"
#include "led_device.h"

void CAL_GPIOLedDevInit(LedDevice *ptLedDevice)
{
	/*STD标准库／HAL库的芯片: 使用STD_GPIOButtonConfig*/
	STD_GPIOLedDevConfig(ptLedDevice->iPort,ptLedDevice->iPin);

	/*其他芯片*/
}

void CAL_GPIOLedDevControl(LedDevice *ptLedDevice,int istatus)
{
	/*STD标准库／HAL库的芯片: 使用STD_GPIOButtonConfig*/
	STD_GPIOLedDevControl(ptLedDevice->iPort,ptLedDevice->iPin,istatus);

	/*其他芯片*/
}


