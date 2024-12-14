#include "cal_button.h" 

void KAL_GPIOButtonInit(int button_pin)
{
	/*对于裸机: 使用CAL*/
	CAL_GPIOButtonInit(button_pin);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
    //rt_pin_mode(button->pin, PIN_MODE_INPUT_PULLUP);
}

