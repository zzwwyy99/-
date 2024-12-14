#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stm32f10x.h>

#include "drive_gpio.h"
#include "drive_delay.h"
#include "dht11_device.h"

//���ź궨��
#define	DHT11_PORT			GPIOA
#define	DHT11_PIN			GPIO_Pin_3
#define	DHT11_PERIPH_RCC	RCC_APB2Periph_GPIOA

//д0������������
#define BUS_WRITE(x)    GPIO_WriteBit(DHT11_PORT,DHT11_PIN,(BitAction)x)	
#define BUS_READ()	    GPIO_ReadInputDataBit(GPIOA,DHT11_PIN)


//ʡ��KAL,CAL.
void DHT11Init(void)
{
	STD_DHT11GpioConfigure(DHT11_PORT,DHT11_PIN);
}

/* �������͸�λ�ź� */
static void DHT11Start(void)
{
	//��һ��40bit���ݴ�����Ϻ�,DHT11����������50us,    \
			���Դ˴���ʱ55us,�Է���һ�δ��������,�´δ�������ʼ
	Delay_10us(6);      //Delay_us(55);
	
	//��������
	BUS_WRITE(0);
	
	//��ʱ����18ms
	Delay_ms(20);
	
	//�ͷ�����
	BUS_WRITE(1);
	
	//��ʱ30us��,���ӻ�DHT11�Ƿ�����Ӧ
	Delay_10us(3);      //Delay_us(30);
}

/* ��������DHT11��Ӧ�ź� */
//����ֵΪ1��ʾ��������,����Ӧ
static char DHT11ReceiveAck(void)
{
	int retry = 0;
	
	//ֵΪ0ʱ,��ʾ����Ӧ
	while(BUS_READ() && retry<100){
		retry++;
		Delay_10us(1);
	}

	if(retry < 100){
		while(BUS_READ() == 0);		//�ȴ�80us�ĵ͵�ƽ��ȥ
		while(BUS_READ() == 1);		//�ȴ�80us�ĸߵ�ƽ��ȥ
		return 1;
	}else{
		return 0;
	}
}

/* ��������һ���ֽ� */
static char DHT11ReceiveByte(void)
{
	unsigned char i = 0;
	unsigned char byte = 0;
	
	//��ȡ���߰˴�
	for(i=0;i<8;i++){
		while(BUS_READ() == 0);		//�ȴ�50us��0��ƽ����
		
		Delay_10us(3);					//��ʱ30us
		
		byte = byte<<1;
		if(BUS_READ() == 1){			//�����ʱ����1,��Ϊ1
			byte |= 1;
		}

//      while(BUS_READ() == 1);		//�ȴ��ߵ�ƽ����
        //��ʱ����ʱ10us,���Ǻܾ�׼,�����һ���ֽڴ����ʱ��,��һֱΪ1,�Ӷ�����.
        Delay_10us(4);  
	}
	
	return byte;
}


/*��������һ������*/
int DHT11ReceiveData(char *data)
{
	DHT11Start();
	if(DHT11ReceiveAck()){
		data[0] = DHT11ReceiveByte();
		data[1] = DHT11ReceiveByte();
		data[2] = DHT11ReceiveByte();
		data[3] = DHT11ReceiveByte();
        data[4] = DHT11ReceiveByte();	//����5������
    }
	if(data[0] + data[1] + data[2] + data[3]  == data[4]){	//����У��
        printf("dht11 ok.\r\n");
		return 1;
	}else{
        printf("dht11 fail.\r\n");
		return 0;
	}
}

