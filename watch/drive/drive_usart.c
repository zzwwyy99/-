#include <stm32f10x.h>
#include <stdio.h>

#include "drive_usart.h"

//����һ������1ʹ�õ�ring_buffer
ring_buffer  usart1_ring_buffer;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /*Ƕ�������жϿ�������ѡ��*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /*����USARTΪ�ж�Դ*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /*�������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /*�����ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /*ʹ���ж�*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /*��ʼ������NVIC*/
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ���ô��ڲ���
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*�򿪴���GPIO��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	/*�򿪴��������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/*������Tx:PA9 ��GPIO����Ϊ���츴��ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*������Rx:PA10 ����Ϊ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*���ô��ڵĹ�������*/ 
	USART_InitStructure.USART_BaudRate = 115200;					//���ò�����	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�����������ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 			//����ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;			//����У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //����Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//���ù���ģʽ���շ�һ��
	USART_Init(USART1, &USART_InitStructure);						//��ɴ��ڵĳ�ʼ������

	NVIC_Configuration();   	//�����ж����ȼ�����
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);    //ʹ���ж�Դ:���ڽ����ж�
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���
}


/* ����һ���ֽ� */
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/*����һ���ֽ����ݵ�USART*/
	USART_SendData(pUSARTx,ch);
		
	/*�ȴ��������ݼĴ���Ϊ��*/
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/* �����ַ��� */
void Usart_SendString(USART_TypeDef * pUSARTx, char *str)
{
	unsigned int i = 0;
	do{
	  Usart_SendByte(pUSARTx,*(str + i));
	  i++;
	}while(*(str + i)!='\0');

	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

 ///�ض���c�⺯��printf������,�ض�����ʹ��printf����
int fputc(int ch,FILE *f)
{
	//��ʹ��printfʱ��ʹ�ô��ڷ�����Ҫ���������
	Usart_SendByte(USART1,ch);
	
	return ch;
}

//�ض���c�⺯��scanf������,��д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	//��ring_buffer��ȡ����
	char c;
	while(ring_buffer_read(&c,&usart1_ring_buffer) != 0); //��ȡû����

	return c;
}

/* ���ڽ����ж� */
void USART1_IRQHandler(void)
{
	/* ���ַ�����ring_buffer */
	ring_buffer_write(USART_ReceiveData(USART1), &usart1_ring_buffer);

	//��������жϱ�־λ
	//USART_ClearFlag(USART1,USART_FLAG_RXNE);
}

