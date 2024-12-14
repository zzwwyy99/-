#include <stm32f10x.h>

#include "drive_pwm.h"
#include "beep_device.h"

void CAL_HardwareBeepDevInit(BeepDevice *ptBeepDevice)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_GPIOBeepDevConfig(ptBeepDevice->iPort,ptBeepDevice->iPin);

	/* ���������Ǳ�׼��оƬ */
}

void CAL_BeepDevicePWMCfg(BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_BeepDevicePWMCfg(ptBeepDevice->timer,period,pulse);

	/* ��������оƬ */
}

void CAL_BeepDevicePWMOn(BeepDevice *ptBeepDevice)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_BeepDevicePWMOff(ptBeepDevice->timer);

	/* ��������оƬ */
}

void CAL_BeepDevicePWMOff(BeepDevice *ptBeepDevice)
{
	/* ����HAL��ͱ�׼��: ʹ��STD */
	STD_BeepDevicePWMOff(ptBeepDevice->timer);

	/* ��������оƬ */
}


