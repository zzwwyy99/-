#include "button_device.h"
#include "button.h"

void AddButtonDevice(void)
{	
	/*初始化按键管理器*/
	__ButtonDevcieManagerInit();
	
	/*添加3个按键KEY*/
	AddButtonDeviceKEY();
}

/*获得一个按键*/
PButton GetButtonDevice(char *name)
{
	return __GetButton(name);
}


