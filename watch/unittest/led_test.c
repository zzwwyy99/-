#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"
#include "led_system.h"

void led_test(void)
{
	//初始化按键
	AddLedDevice();
	
	PLedDevice ptledwhite = GetLedDevice("WHITE");
    PLedDevice ptledred = GetLedDevice("RED");
	//注册的时候应该已经初始化了
	//ptled->init();

	ptledwhite->control(ptledwhite,LED_ON);
    ptledred->control(ptledred,LED_ON);
    
    delay(50000);
    
    ptledwhite->control(ptledwhite,LED_OFF);
    ptledred->control(ptledred,LED_OFF);
}

