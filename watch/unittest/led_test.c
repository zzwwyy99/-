#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"
#include "led_system.h"

void led_test(void)
{
	//��ʼ������
	AddLedDevice();
	
	PLedDevice ptledwhite = GetLedDevice("WHITE");
    PLedDevice ptledred = GetLedDevice("RED");
	//ע���ʱ��Ӧ���Ѿ���ʼ����
	//ptled->init();

	ptledwhite->control(ptledwhite,LED_ON);
    ptledred->control(ptledred,LED_ON);
    
    delay(50000);
    
    ptledwhite->control(ptledwhite,LED_OFF);
    ptledred->control(ptledred,LED_OFF);
}

