#include "button_device.h"
#include "button.h"

void AddButtonDevice(void)
{	
	/*��ʼ������������*/
	__ButtonDevcieManagerInit();
	
	/*���3������KEY*/
	AddButtonDeviceKEY();
}

/*���һ������*/
PButton GetButtonDevice(char *name)
{
	return __GetButton(name);
}


