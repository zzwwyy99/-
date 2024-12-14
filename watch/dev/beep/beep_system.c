#include "beep_device.h"
#include "beep.h"

void ADDBeepDevice(void)
{
	/* 添加设备到设备管理器 */
	AddBeepDeviceBeep();
    
    /* */
}

/*获得一个设备*/
PBeepDevice GetBeepDevice(char *name)
{
	return __GetBeepDevice(name);
}

