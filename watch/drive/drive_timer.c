#include <stm32f10x.h>
#include <stdio.h>

#include "drive_timer.h"
#include "drive_delay.h"
#include "display_system.h"

/* ���嶨ʱ���жϴ����� */
static TimerProcessFunction g_fTimerProcessFunction;

void SetTimerProcessFunction(TimerProcessFunction func)
{
	g_fTimerProcessFunction = func;
}
	
/**
* @brief  ��ʼ����ʱ��2,��ʱ������Ϊ10ms
* @param  ��
* @retval ��
* @note   	  
*/
void STD_TimerConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//���ö�ʱ��2
	NVIC_InitTypeDef NVIC_InitStruct;						//�ж�����
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);		//������ʱ��2ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

	/*ʹ�ܶ�ʱ���ж�*/
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_Cmd(TIM2,ENABLE);
}

extern int flush_start;
/**
* @brief  ��ʱ���ж�:���а���ɨ��
* @param  ��
* @retval ��
*/
void TIM2_IRQHandler(void)
{    
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)){
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

        //printf("��ʱ���ص�\r\n");
        /* ����ʽ��� */
        if(g_fTimerProcessFunction){
            g_fTimerProcessFunction();
        }
        
        if(flush_start == 1){
            TimerFlushDisplayDev();
        }
	}
}


/**
* @brief  ��ʼ����ʱ��3,��ʱ������Ϊ10us
* @param  ��
* @retval ��
* @note   	  
*/
void STD_Timer3Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//���ö�ʱ��2
	NVIC_InitTypeDef NVIC_InitStruct;						//�ж�����
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		//������ʱ��2ʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStruct);

	/*ʹ�ܶ�ʱ���ж�*/
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler(void)
{    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)){
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
        
        TimingDelay_Decrement();
	}
}
