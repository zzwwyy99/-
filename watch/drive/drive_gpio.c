#include <stm32f10x.h>

/* 直接操作寄存器的方法控制IO */
#define LED_ON_REG(p,i)     {p->BRR=i;}	    //输出低电平
#define	LED_OFF_REG(p,i)    {p->BSRR=i;}	//输出为高电平		
#define LED_TOGGLE_REG(p,i) {p->ODR ^=i;} 	//输出反转状态

/**
* @brief  根据按键的pin配置GPIO_pin
* @param  按键的pin
* @retval 无
* @note   
*/
void STD_GPIOButtonConfig(uint16_t button_pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    
    //开启GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_InitStruct.GPIO_Pin = button_pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}


/**
* @brief  根据led的pin配置GPIO_pin
* @param  LEDDevice的pin
* @retval 无
* @note   
*/
void STD_GPIOLedDevConfig(void *port,int pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    
    //开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输入
    GPIO_InitStruct.GPIO_Pin = pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)port,&GPIO_InitStruct);
}

//控制LED状态
void STD_GPIOLedDevControl(void *port,int pin,int istatus)
{
	switch(istatus){
		case 0:
			LED_ON_REG(((GPIO_TypeDef *)port),pin);
			break;
		case 1:
			LED_OFF_REG(((GPIO_TypeDef *)port),pin);
			break;
		case 2:
			LED_TOGGLE_REG(((GPIO_TypeDef *)port),pin);
			break;
		default:
			break;
	}
	
}


void STD_DHT11GpioConfigure(void *port,int pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    
    //开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;		//配置为开漏输出,写0时控制总线,写1时释放总线,类似于IIC
    GPIO_InitStruct.GPIO_Pin = pin;						//PA3
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)port,&GPIO_InitStruct);
    
    
}


