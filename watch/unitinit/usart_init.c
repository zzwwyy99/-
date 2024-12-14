#include <stm32f10x.h>
#include <stdio.h>

#include "drive_usart.h" 
#include "ring_buffer.h" 

void UsartInit(void) 
{
	char c = 'A';
	char *s = "100ask wds";
    //char getstr[10];
    
	//����usart1
	USART_Config();
	
    //��ʼ�����λ�����
    ring_buffer_init(&usart1_ring_buffer);
    
	//����ʮ���ֽ�
	for(;c <= 'Z';c++){
		Usart_SendByte(USART1,c);
	}
    
    //�����ַ���"100ask"
    Usart_SendString(USART1,"\r\n100ask\r\n");
    
    //��׼���
    printf("nb,%s\r\n",s);
    printf("���ڳ�ʼ�����.\r\n");
	
    //���Ի��λ�������д
//    while(1)
//    while(ring_buffer_read(&c,&usart1_ring_buffer) == 0)
//        printf("%c",c);
}
   
