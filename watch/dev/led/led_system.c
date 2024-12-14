#include "led_device.h"
#include "led.h"

void AddLedDevice(void)
{
	/* 初始化Led_device管理器 */
	//不同于Button_Device管理器,Led_device管理器不需要初始化.

	/* 添加Led设备 */
	AddLedDeviceToManager();
}

PLedDevice GetLedDevice(char *name)
{
	return __GetLedDevice(name);
}

