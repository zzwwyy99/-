#include "beep_device.h"
#include "beep.h"

void ADDBeepDevice(void)
{
	/* ����豸���豸������ */
	AddBeepDeviceBeep();
    
    /* */
}

/*���һ���豸*/
PBeepDevice GetBeepDevice(char *name)
{
	return __GetBeepDevice(name);
}

