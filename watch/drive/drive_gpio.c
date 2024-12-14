#include <stm32f10x.h>

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define LED_ON_REG(p,i)     {p->BRR=i;}	    //����͵�ƽ
#define	LED_OFF_REG(p,i)    {p->BSRR=i;}	//���Ϊ�ߵ�ƽ		
#define LED_TOGGLE_REG(p,i) {p->ODR ^=i;} 	//�����ת״̬

/**
* @brief  ���ݰ�����pin����GPIO_pin
* @param  ������pin
* @retval ��
* @note   
*/
void STD_GPIOButtonConfig(uint16_t button_pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    
    //����GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_InitStruct.GPIO_Pin = button_pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}


/**
* @brief  ����led��pin����GPIO_pin
* @param  LEDDevice��pin
* @retval ��
* @note   
*/
void STD_GPIOLedDevConfig(void *port,int pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
    
    //����GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//��������
    GPIO_InitStruct.GPIO_Pin = pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)port,&GPIO_InitStruct);
}

//����LED״̬
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
    
    //����GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;		//����Ϊ��©���,д0ʱ��������,д1ʱ�ͷ�����,������IIC
    GPIO_InitStruct.GPIO_Pin = pin;						//PA3
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init((GPIO_TypeDef *)port,&GPIO_InitStruct);
    
    
}


