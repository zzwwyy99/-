#include <stm32f10x.h>
#include <stdio.h>

#include "drive_usart.h"

//定义一个串口1使用的ring_buffer
ring_buffer  usart1_ring_buffer;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /*嵌套向量中断控制器组选择*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /*配置USART为中断源*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /*抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /*子优先级*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /*使能中断*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /*初始化配置NVIC*/
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  配置串口参数
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*打开串口GPIO的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*打开串口外设的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/*将串口Tx:PA9 的GPIO配置为推挽复用模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*将串口Rx:PA10 配置为浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*配置串口的工作参数*/ 
	USART_InitStructure.USART_BaudRate = 115200;					//配置波特率	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//配置针数据字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 			//配置停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;			//配置校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //配置硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//配置工作模式，收发一起
	USART_Init(USART1, &USART_InitStructure);						//完成串口的初始化配置

	NVIC_Configuration();   	//串口中断优先级配置
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);    //使能中断源:串口接受中断
	USART_Cmd(USART1, ENABLE);  //使能串口
}


/* 发送一个字节 */
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/*发送一个字节数据到USART*/
	USART_SendData(pUSARTx,ch);
		
	/*等待发送数据寄存器为空*/
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/* 发送字符串 */
void Usart_SendString(USART_TypeDef * pUSARTx, char *str)
{
	unsigned int i = 0;
	do{
	  Usart_SendByte(pUSARTx,*(str + i));
	  i++;
	}while(*(str + i)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

 ///重定向c库函数printf到串口,重定向后可使用printf函数
int fputc(int ch,FILE *f)
{
	//当使用printf时，使用串口发送需要输出的数据
	Usart_SendByte(USART1,ch);
	
	return ch;
}

//重定向c库函数scanf到串口,重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	//从ring_buffer读取数据
	char c;
	while(ring_buffer_read(&c,&usart1_ring_buffer) != 0); //读取没问题

	return c;
}

/* 串口接受中断 */
void USART1_IRQHandler(void)
{
	/* 将字符存入ring_buffer */
	ring_buffer_write(USART_ReceiveData(USART1), &usart1_ring_buffer);

	//清除接受中断标志位
	//USART_ClearFlag(USART1,USART_FLAG_RXNE);
}

