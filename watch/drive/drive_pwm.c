#include <stm32f10x.h>
#include "drive_pwm.h"

/**
* @brief  配置定时器4的输出通道的GPIO
* @param  无
* @retval 无
*/
void STD_GPIOBeepDevConfig(void *port,int pin) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

	//输出比较通道1 GPIO初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //PB8,TIM4,CH3
    GPIO_InitStructure.GPIO_Pin = pin;
    
    /* 如果把端口配置成复用输出功能，则引脚和输出寄存器断开，并和片上外设的输出信号连接。
        将GPIO脚配置成复用输出功能，且定时器没有使能时,将一直输出低电平 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //复用推挽输出            
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(port, &GPIO_InitStructure);
    
    //复用,不知道能不能输出高电平:不能
    //GPIO_SetBits(port,pin);
}

/**
* @brief  初始化Timer4
* @param  无
* @retval 无
*/
//Tout = 1/fout = (arr+1)*(psc+1)/Tclk = 1/262k
void STD_BeepDevicePWMCfg(void *timer,unsigned int period,unsigned int pulse)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	
    /* 时基结构体初始化 */
    TIM_TimeBaseInitStruct.TIM_Period = 999;			//ARR的值
    TIM_TimeBaseInitStruct.TIM_Prescaler = period;		//period,周期
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(timer,&TIM_TimeBaseInitStruct);
	
    /* 输出比较结构体初始化 */	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;		//蜂鸣器低电平有效
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = pulse;							//pulse,占空比
    TIM_OC3Init(timer,&TIM_OCInitStruct);
    TIM_OC3PreloadConfig(timer,TIM_OCPreload_Enable);     
   
    TIM_Cmd(timer,ENABLE);	//使能定时器
}  

void STD_BeepDevicePWMOff(void *timer)
{
    STD_BeepDevicePWMCfg(timer,262,0);
}   



