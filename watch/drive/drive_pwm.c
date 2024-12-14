#include <stm32f10x.h>
#include "drive_pwm.h"

/**
* @brief  ���ö�ʱ��4�����ͨ����GPIO
* @param  ��
* @retval ��
*/
void STD_GPIOBeepDevConfig(void *port,int pin) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

	//����Ƚ�ͨ��1 GPIO��ʼ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //PB8,TIM4,CH3
    GPIO_InitStructure.GPIO_Pin = pin;
    
    /* ����Ѷ˿����óɸ���������ܣ������ź�����Ĵ����Ͽ�������Ƭ�����������ź����ӡ�
        ��GPIO�����óɸ���������ܣ��Ҷ�ʱ��û��ʹ��ʱ,��һֱ����͵�ƽ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������            
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(port, &GPIO_InitStructure);
    
    //����,��֪���ܲ�������ߵ�ƽ:����
    //GPIO_SetBits(port,pin);
}

/**
* @brief  ��ʼ��Timer4
* @param  ��
* @retval ��
*/
//Tout = 1/fout = (arr+1)*(psc+1)/Tclk = 1/262k
void STD_BeepDevicePWMCfg(void *timer,unsigned int period,unsigned int pulse)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	
    /* ʱ���ṹ���ʼ�� */
    TIM_TimeBaseInitStruct.TIM_Period = 999;			//ARR��ֵ
    TIM_TimeBaseInitStruct.TIM_Prescaler = period;		//period,����
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(timer,&TIM_TimeBaseInitStruct);
	
    /* ����ȽϽṹ���ʼ�� */	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;		//�������͵�ƽ��Ч
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = pulse;							//pulse,ռ�ձ�
    TIM_OC3Init(timer,&TIM_OCInitStruct);
    TIM_OC3PreloadConfig(timer,TIM_OCPreload_Enable);     
   
    TIM_Cmd(timer,ENABLE);	//ʹ�ܶ�ʱ��
}  

void STD_BeepDevicePWMOff(void *timer)
{
    STD_BeepDevicePWMCfg(timer,262,0);
}   



