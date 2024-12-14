#include <stm32f10x.h>
#include <stdio.h>

#include "drive_usart.h" 
#include "ring_buffer.h" 

void UsartInit(void) 
{
	char c = 'A';
	char *s = "100ask wds";
    //char getstr[10];
    
	//配置usart1
	USART_Config();
	
    //初始化环形缓冲区
    ring_buffer_init(&usart1_ring_buffer);
    
	//发送十个字节
	for(;c <= 'Z';c++){
		Usart_SendByte(USART1,c);
	}
    
    //发送字符串"100ask"
    Usart_SendString(USART1,"\r\n100ask\r\n");
    
    //标准输出
    printf("nb,%s\r\n",s);
    printf("串口初始化完成.\r\n");
	
    //测试环形缓冲区读写
//    while(1)
//    while(ring_buffer_read(&c,&usart1_ring_buffer) == 0)
//        printf("%c",c);
}
   
