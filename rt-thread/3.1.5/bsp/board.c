/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-24                  the first version
 */
#include "stm32f10x.h"  

#include <rthw.h>
#include <rtthread.h>

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
/*
 * Please modify RT_HEAP_SIZE if you enable RT_USING_HEAP
 * the RT_HEAP_SIZE max value = (sram size - ZI size), 1024 means 1024 bytes
 */
#define RT_HEAP_SIZE (6*1024)
static rt_uint8_t rt_heap[RT_HEAP_SIZE];

RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

void rt_os_tick_callback(void)
{
    rt_interrupt_enter();
    
    rt_tick_increase();

    rt_interrupt_leave();
}

/**
 * This function will initial your board.
 */
extern void BoradInit(void);

void rt_hw_board_init(void)
{
//#error "TODO 1: OS Tick Configuration."
    /* 
     * TODO 1: OS Tick Configuration
     * Enable the hardware timer and call the rt_os_tick_callback function
     * periodically with the frequency RT_TICK_PER_SECOND. 
     */ 
   	/* ϵͳ���ʱ��,ʱ�������������ļ������ú��� */
    SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );	
    
    /* ��ʼ��Ӳ�� */
    BoradInit();
    
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
    /* �����ж� */
    rt_interrupt_enter();

    /* ����ʱ�� */
    rt_tick_increase();

    /* �뿪�ж� */
    rt_interrupt_leave();
}


#ifdef RT_USING_CONSOLE

/**
  * @brief  ��ӳ�䴮��DEBUG_USARTx��rt_kprintf()����
  *   Note��DEBUG_USARTx����bsp_usart.h�ж���ĺ꣬Ĭ��ʹ�ô���1
  * @param  str��Ҫ��������ڵ��ַ���
  * @retval ��
  *
  * @attention
  * 
  */
void rt_hw_console_output(const char *str)
{	
	/* �����ٽ�� */
    rt_enter_critical();

	/* ֱ���ַ������� */
    while (*str!='\0')
	{
		/* ���� */
        if (*str=='\n')
		{
			USART_SendData(USART1, '\r'); 
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}

		USART_SendData(USART1, *str++); 				
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	}	

	/* �˳��ٽ�� */
    rt_exit_critical();
}

//static int uart_init(void)
//{
//#error "TODO 2: Enable the hardware uart and config baudrate."
//    return 0;
//}
//INIT_BOARD_EXPORT(uart_init);

//void rt_hw_console_output(const char *str)
//{
//#error "TODO 3: Output the string 'str' through the uart."
//}

#endif

