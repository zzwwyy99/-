#include "drive_gpio.h"

void CAL_GPIOButtonInit(int button_pin)
{
	/*STD标准库／HAL库的芯片: 使用STD_GPIOButtonConfig*/
	STD_GPIOButtonConfig(button_pin);

	/*其他芯片*/
}


