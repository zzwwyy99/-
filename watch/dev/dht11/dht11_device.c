#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stm32f10x.h>

#include "drive_gpio.h"
#include "drive_delay.h"
#include "dht11_device.h"

//引脚宏定义
#define	DHT11_PORT			GPIOA
#define	DHT11_PIN			GPIO_Pin_3
#define	DHT11_PERIPH_RCC	RCC_APB2Periph_GPIOA

//写0即是拉低总线
#define BUS_WRITE(x)    GPIO_WriteBit(DHT11_PORT,DHT11_PIN,(BitAction)x)	
#define BUS_READ()	    GPIO_ReadInputDataBit(GPIOA,DHT11_PIN)


//省略KAL,CAL.
void DHT11Init(void)
{
	STD_DHT11GpioConfigure(DHT11_PORT,DHT11_PIN);
}

/* 主机发送复位信号 */
static void DHT11Start(void)
{
	//在一次40bit数据传送完毕后,DHT11会拉低总线50us,    \
			所以此处延时55us,以防上一次传输结束后,下次传输立马开始
	Delay_10us(6);      //Delay_us(55);
	
	//拉低总线
	BUS_WRITE(0);
	
	//延时至少18ms
	Delay_ms(20);
	
	//释放总线
	BUS_WRITE(1);
	
	//延时30us后,检测从机DHT11是否有响应
	Delay_10us(3);      //Delay_us(30);
}

/* 主机接收DHT11响应信号 */
//返回值为1表示拉低总线,有响应
static char DHT11ReceiveAck(void)
{
	int retry = 0;
	
	//值为0时,表示有响应
	while(BUS_READ() && retry<100){
		retry++;
		Delay_10us(1);
	}

	if(retry < 100){
		while(BUS_READ() == 0);		//等待80us的低电平过去
		while(BUS_READ() == 1);		//等待80us的高电平过去
		return 1;
	}else{
		return 0;
	}
}

/* 主机接收一个字节 */
static char DHT11ReceiveByte(void)
{
	unsigned char i = 0;
	unsigned char byte = 0;
	
	//读取总线八次
	for(i=0;i<8;i++){
		while(BUS_READ() == 0);		//等待50us的0电平结束
		
		Delay_10us(3);					//延时30us
		
		byte = byte<<1;
		if(BUS_READ() == 1){			//如果此时还是1,则为1
			byte |= 1;
		}

//      while(BUS_READ() == 1);		//等待高电平结束
        //定时器延时10us,不是很精准,在最后一个字节传输的时候,会一直为1,从而卡死.
        Delay_10us(4);  
	}
	
	return byte;
}


/*主机接收一次数据*/
int DHT11ReceiveData(char *data)
{
	DHT11Start();
	if(DHT11ReceiveAck()){
		data[0] = DHT11ReceiveByte();
		data[1] = DHT11ReceiveByte();
		data[2] = DHT11ReceiveByte();
		data[3] = DHT11ReceiveByte();
        data[4] = DHT11ReceiveByte();	//接收5个数据
    }
	if(data[0] + data[1] + data[2] + data[3]  == data[4]){	//数据校验
        printf("dht11 ok.\r\n");
		return 1;
	}else{
        printf("dht11 fail.\r\n");
		return 0;
	}
}

