#include "cal_button.h" 

void KAL_GPIOButtonInit(int button_pin)
{
	/*�������: ʹ��CAL*/
	CAL_GPIOButtonInit(button_pin);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
    //rt_pin_mode(button->pin, PIN_MODE_INPUT_PULLUP);
}

