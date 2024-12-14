#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"

/* 软件延时 */

 void delay(int time)
{
    int i, j;
	
	for (i = 0; i < time; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
}


/* 定时器延时 */
static volatile int TimingDelay;

/**
  * @brief   us延时程序,10us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
  * @retval  无
  */
void Delay_10us(volatile int nTime)
{ 
	TimingDelay = nTime;	

	// 使能定时器  
	TIM_Cmd(TIM3,ENABLE);

	while(TimingDelay != 0){
        ;
    }
    
    TIM_Cmd(TIM3,DISABLE);
}

void Delay_ms(volatile int nTime)
{    
    //n ms.
    for(int i=0;i<nTime;i++){
        Delay_10us(100); //1ms
        //printf("%d\r\n",i);
    }
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在 定时器 中断函数中调用
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

