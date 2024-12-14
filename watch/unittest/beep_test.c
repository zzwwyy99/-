#include <stm32f10x.h>
#include <stdio.h>

#include "beep_system.h"
#include "drive_delay.h"

void beep_test(void)
{
//    uint16_t freq_tab[12]  = {262, 277, 294, 311, 330, 349, 369, 392, 415, 440, 466, 494}; //原始频率表 CDEFGAB
    uint16_t freq_tab2[7]  = {523, 587, 659, 699, 784, 880, 988};

	PBeepDevice pdev;

	//添加beepdev
	ADDBeepDevice();

	//获得beep设备
	pdev = GetBeepDevice("beep");

	//已经初始化了硬件

	//配置beep_pwm且ON    
    for(int i =0;i<7;i++){
        pdev->pwm_cfg(pdev,freq_tab2[i],40);
    
        delay(4000);
    
        pdev->pwm_off(pdev);
    
        delay(4000);
    
    }
    
    
}

