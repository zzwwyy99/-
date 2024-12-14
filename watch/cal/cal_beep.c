#include <stm32f10x.h>

#include "drive_pwm.h"
#include "beep_device.h"

void CAL_HardwareBeepDevInit(BeepDevice *ptBeepDevice)
{
	/* 对于HAL库和标准库: 使用STD */
	STD_GPIOBeepDevConfig(ptBeepDevice->iPort,ptBeepDevice->iPin);

	/* 对于其他非标准库芯片 */
}

void CAL_BeepDevicePWMCfg(BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse)
{
	/* 对于HAL库和标准库: 使用STD */
	STD_BeepDevicePWMCfg(ptBeepDevice->timer,period,pulse);

	/* 对于其他芯片 */
}

void CAL_BeepDevicePWMOn(BeepDevice *ptBeepDevice)
{
	/* 对于HAL库和标准库: 使用STD */
	STD_BeepDevicePWMOff(ptBeepDevice->timer);

	/* 对于其他芯片 */
}

void CAL_BeepDevicePWMOff(BeepDevice *ptBeepDevice)
{
	/* 对于HAL库和标准库: 使用STD */
	STD_BeepDevicePWMOff(ptBeepDevice->timer);

	/* 对于其他芯片 */
}


