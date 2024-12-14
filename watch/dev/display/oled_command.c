#include <stm32f10x.h>
#include <stdio.h>
#include <rtthread.h>
#include <rthw.h>

#include "i2c_system.h"

#define	OLED_ADDR	0x78

//#define	OLEDInterfaceDev	I2CDevice
static I2CDevice *g_ptI2CDevice;

int OLED_I2C1_DMA_BUSYFLAG = 0;//DMAæµ��־λ,ֵΪ1:DMA���ڴ�������,ע���ʱ���ܸ���GRAM�е�ֵ

//��������õ�һ��I2CDevice,����������i2c.c��������ֱ�ӹ���һ��I2CDevice��,����GetI2CDevice("i2c1"),ֱ�ӻ��һ��i2c�豸
static int OLEDInfDevInit(void *frambuffer,int buffersize)
{
	/* ��ʼ��i2c_system */
	ADDI2CDevice();
	
	/* ���I2C1�豸 */
	g_ptI2CDevice = GetI2CDevice("i2c1");
	g_ptI2CDevice->iDMAmemBaseAddr = (int)frambuffer;
	g_ptI2CDevice->iBufferSize = buffersize;
	
    g_ptI2CDevice->dam_cfg(g_ptI2CDevice);	//��������DMA
	return 0;
}

/**
* @brief  �����豸��ַ-����Ҫд��ļĴ���addr-����Ҫд��Ĵ���������data
* @param  addr:д�����ݵ�oled�Ĵ����ĵ�ַ
          data:д��oled�Ĵ���addr������
* @retval ��
*/
//OLED i2cͨ�Ź涨,�ȷ���oled�ĵ�ַ,�ٷ���oled�Ĵ����ĵ�ַ,�ٷ��͵�oled�Ĵ���������
static void OLEDByteSend(unsigned char reg_addr,unsigned char data) 
{
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));							//���I2C�Ƿ�æ
    
    I2C_GenerateSTART(I2C1,ENABLE);											//����I2C,������ʼ�ź�
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));				//�¼�5����,�����Ѳ�����ʼ�ź�

    I2C_Send7bitAddress(I2C1,OLED_ADDR,I2C_Direction_Transmitter);			//����Oled�豸��ַ:0x78
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//�¼�6����,�����ҵ���ַΪ0x78������
    
    I2C_SendData(I2C1,reg_addr);												//I2C1����addr��Oled
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));		//EV8
    
    I2C_SendData(I2C1,data);												//I2C1�������ݵ�Oled
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//EV8_2
    
    I2C_GenerateSTOP(I2C1,ENABLE);
}

/**
* @brief  �������ݵ�oled��0x00�Ĵ���
* @param  cmd:0x00�Ĵ�����ݼ�����
* @retval ��
*/
void OLEDCommandSend(unsigned char cmd)
{
    OLEDByteSend(0x00,cmd);
}

/**
* @brief  д����
* @param  I2C_Data:����
* @retval ��
*/
void OLEDDataSend(unsigned char data)
{
    OLEDByteSend(0x40,data);
}


/************** 1. ��������ܺ��� **************/
/**
* @brief  ����Oled�Ĺ��λ��
* @param  x:��λ��,ֵΪ0~127
          y:��λ��,ֵΪ0~7
* @retval ��
*/
void oled_setcursor(int x,int y)
{
    OLEDCommandSend(0xb0 | y);
    OLEDCommandSend(0x10 |(x & 0xF0) >> 4);		//ȡx�ĸ���λ
    OLEDCommandSend(0x00 |(x & 0x0F));			//ȡx�ĵ���λ
}

/************** 7. OLED��ʼ������ **************/
/*oled��Ļ��ʼ��*/
void oled_init(char *pFramBuffer,int iSize)
{
    int i,j;
	
	/*��ʼ��oled��IIC�ӿ�*/
	OLEDInfDevInit(pFramBuffer,iSize);
	
	/*�ϵ���ʱ*/
	for (i = 0; i < 1000; i++)		
	{
		for (j = 0; j < 1000; j++);
	}
	
	/*oled����*/
	OLEDCommandSend(0xAE); //display off
	OLEDCommandSend(0x20);	//Set Memory Addressing Mode	
	OLEDCommandSend(0x00);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLEDCommandSend(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLEDCommandSend(0xc8);	//Set COM Output Scan Direction
	OLEDCommandSend(0x00); //---set low column address
	OLEDCommandSend(0x10); //---set high column address
	OLEDCommandSend(0x40); //--set start line address
	OLEDCommandSend(0x81); //--set contrast control register
	OLEDCommandSend(0xff); //--set 0x00~0xff
	OLEDCommandSend(0xa1); //--set segment re-map 0 to 127
	OLEDCommandSend(0xa6); //--set normal display
	OLEDCommandSend(0xa8); //--set multiplex ratio(1 to 64)
	OLEDCommandSend(0x3F); //
	OLEDCommandSend(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLEDCommandSend(0xd3); //-set display offset
	OLEDCommandSend(0x00); //-not offset
	OLEDCommandSend(0xd5); //--set display clock divide ratio/oscillator frequency
	OLEDCommandSend(0xf0); //--set divide ratio
	OLEDCommandSend(0xd9); //--set pre-charge period
	OLEDCommandSend(0x22); //
	OLEDCommandSend(0xda); //--set com pins hardware configuration
	OLEDCommandSend(0x12);	//
	OLEDCommandSend(0xdb); //--set vcomh
	OLEDCommandSend(0x20); //0x20,0.77xVcc
	OLEDCommandSend(0x8d); //--set DC-DC enable
	OLEDCommandSend(0x14); //
	OLEDCommandSend(0xaf); //--turn on oled panel
	
	/*��ʼ��IIC��DMA*/
    //g_ptI2CDevice->dam_cfg(g_ptI2CDevice);	//���DMA����
	//I2C_DMACmd(I2C1,ENABLE);				//����DMA����,�������ֻ��Ҫ����һ��
}


/************** 8. �����������ܺ��� **************/
/**
* @brief  ����0x00��Oled
* @param  
* @retval ��
*/
void oled_clear(void)
{
    int i;
    
    oled_setcursor(0,0);
    for(i=0;i<1024;i++){
     	OLEDDataSend(0x00);
	}
}

/**
* @brief  ��GRAM�е�ֵ���µ�OLED��
* @param  uint8_t (*gram)[128]:ˢ�½�oled��gramָ��
* @retval ��
* @note   OLED�趨Ϊˮƽģʽ,ֻ��Ҫ�趨һ�ι��λ��
		  һ��ֻ����1���ֽ�
*/
void oled_gram_refresh(unsigned char *gram)
{
	int i;
    
    oled_setcursor(0,0);
    for(i=0;i<1024;i++){
     	OLEDDataSend(*(gram + i));  //�������ΪIIC��ʼ,����һ���ֽ�,IIC�ر�.
	}
}

/**
* @brief  ��DMA����.��GRAM�е�ֵ���µ�OLED��
* @param  uint8_t (*gram)[128]:ˢ�½�oled��gramָ��
* @retval ��
* @note   OLED�趨Ϊˮƽģʽ,ֻ��Ҫ�趨һ�ι��λ��
		  һ�δ���1024���ֽ�
*/
void oled_gram_refresh_onetime(unsigned char *gram)
{
	int i;
	oled_setcursor(0,0);
	
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));							//���I2C�Ƿ�æ
    
    I2C_GenerateSTART(I2C1,ENABLE);											//����I2C,������ʼ�ź�
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));				//�¼�5����,�����Ѳ�����ʼ�ź�
			
    I2C_Send7bitAddress(I2C1,OLED_ADDR,I2C_Direction_Transmitter);			//����oled��ַ 0x78
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//�¼�6����,�����ҵ���ַΪ0x78������
    
    I2C_SendData(I2C1,0x40);												//��addrд��DR�Ĵ���
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));		//EV8
 
	for(i=0;i<1024;i++){
     	I2C_SendData(I2C1,*(gram + i));										//��data��ַд��DR�Ĵ���
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));		//EV8_2
	}
	 
    I2C_GenerateSTOP(I2C1,ENABLE);
}

/**
* @brief  DMA����,��GRAM�е�ֵ���µ�OLED��
* @param  uint8_t (*gram)[128]:ˢ�½�oled��gramָ��
* @retval ��
* @note   OLED�趨Ϊˮƽģʽ,ֻ��Ҫ�趨һ�ι��λ��
		  һ�δ���1024���ֽ�,DMA����
*/
extern int iFPS;
void oled_gram_refresh_dma(void)
{
    rt_interrupt_enter();
	if(OLED_I2C1_DMA_BUSYFLAG == 0){
        //printf("%dmsˢ��һ��\r\n",iFPS*10);
        iFPS = 0;
        
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));							//���I2C�Ƿ�æ
    
		I2C_GenerateSTART(I2C1,ENABLE);											//����I2C,������ʼ�ź�
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));				//�¼�5����,�����Ѳ�����ʼ�ź�
		
		I2C_Send7bitAddress(I2C1,OLED_ADDR,I2C_Direction_Transmitter);			//����oled��ַ 0x78
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//�¼�6����,�����ҵ���ַΪ0x78������
		
		I2C_SendData(I2C1,0x40);												//��addrд��DR�Ĵ���
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));		//EV8_2
    
        rt_interrupt_leave();

		/*��DMA,����DMA�����ݴ�GRAM���˵�I2C->DR.*/
		g_ptI2CDevice->dam_enable(g_ptI2CDevice);

		OLED_I2C1_DMA_BUSYFLAG = 1;				//DMAæ��־λ
	} 
}


//�ȴ�DMA������ɣ�30ms����.
//��д�Դ��ֹ,������Ըĳ��ź���,���������ź����Ϳ��Բ�������.
void WaitForI2CDMABusy(void)
{
    while(OLED_I2C1_DMA_BUSYFLAG);
}

/**
* @brief  DMA��������жϴ�����
* @param  ��
* @retval ��
*/
void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC6)){	//DMA�������
        DMA_ClearFlag(DMA1_FLAG_TC6);		//���������ɱ�־λ
        
        //���I2C_GenerateSTOP����Ҫ��,����I2C��һֱ��BUSY״̬
        I2C_GenerateSTOP(I2C1,ENABLE);		//�ر�I2C
 
        //printf("����һ�����\r\n");       //����I2C_GenerateSTOP()ǰ����Ļ�����ҹ���,���趨DMA_SetCurrDataCounter(DMA_CHx,1024);ʱҲ�����ҹ���.\
                                                Ч����ͬ�ڶ෢����һ���ֽڸ�oled.���ҹ����Ƕ෢����һ���ֽ�,����������ٷ�����һ���ֽ�
 		/* ���ź��������� */
		OLED_I2C1_DMA_BUSYFLAG = 0;			//DMAæµ��־λ,ֵΪ1:DMA���ڴ�������,��ʱû���ٿ���һ�δ���,����ע���ʱ��Ҫ����GRAM�е�ֵ 
	}
}


