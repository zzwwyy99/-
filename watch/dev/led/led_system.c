#include "led_device.h"
#include "led.h"

void AddLedDevice(void)
{
	/* ��ʼ��Led_device������ */
	//��ͬ��Button_Device������,Led_device����������Ҫ��ʼ��.

	/* ���Led�豸 */
	AddLedDeviceToManager();
}

PLedDevice GetLedDevice(char *name)
{
	return __GetLedDevice(name);
}

