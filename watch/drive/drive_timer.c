#include <stm32f10x.h>
#include <stdio.h>

#include "drive_timer.h"
#include "drive_delay.h"
#include "display_system.h"

/* 定义定时器中断处理函数 */
static TimerProcessFunction g_fTimerProcessFunction;

void SetTimerProcessFunction(TimerProcessFunction func)
{
	g_fTimerProcessFunction = func;
}
	
/**
* @brief  初始化定时器2,定时器周期为10ms
* @param  无
* @retval 无
* @note   	  
*/
void STD_TimerConfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//配置定时器2
	NVIC_InitTypeDef NVIC_InitStruct;						//中断配置
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);		//开启定时器2时钟
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

	/*使能定时器中断*/
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	TIM_Cmd(TIM2,ENABLE);
}

extern int flush_start;
/**
* @brief  定时器中断:进行按键扫描
* @param  无
* @retval 无
*/
void TIM2_IRQHandler(void)
{    
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)){
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);

        //printf("定时器回调\r\n");
        /* 防御式编程 */
        if(g_fTimerProcessFunction){
            g_fTimerProcessFunction();
        }
        
        if(flush_start == 1){
            TimerFlushDisplayDev();
        }
	}
}


/**
* @brief  初始化定时器3,定时器周期为10us
* @param  无
* @retval 无
* @note   	  
*/
void STD_Timer3Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;			//配置定时器2
	NVIC_InitTypeDef NVIC_InitStruct;						//中断配置
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		//开启定时器2时钟
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

	/*使能定时器中断*/
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
