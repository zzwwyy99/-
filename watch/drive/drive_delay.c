#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"

/* �����ʱ */

 void delay(int time)
{
    int i, j;
	
	for (i = 0; i < time; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
}


/* ��ʱ����ʱ */
static volatile int TimingDelay;

/**
  * @brief   us��ʱ����,10usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
  * @retval  ��
  */
void Delay_10us(volatile int nTime)
{ 
	TimingDelay = nTime;	

	// ʹ�ܶ�ʱ��  
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
  * @brief  ��ȡ���ĳ���
  * @param  ��
  * @retval ��
  * @attention  �� ��ʱ�� �жϺ����е���
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

