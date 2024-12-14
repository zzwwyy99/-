#ifndef __DRIVE_USART_H
#define __DRIVE_USART_H

#include <stm32f10x.h>
#include "ring_buffer.h"

//定义一个串口1使用的ring_buffer
extern ring_buffer  usart1_ring_buffer;

void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx, char *str);

#endif  /*__DRIVE_USART_H*/

