#include "drive_i2c.h"

/**
* @brief  ��ʼ��I2C1��GPIO����,����I2C1
* @param  ��
* @retval ��
*/
void STD_GPIOI2CDevConfig(void *port,int pin1,int pin2) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;
    
    //����I2C1��GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Pin = pin1 | pin2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(port,&GPIO_InitStruct);

    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 400000;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = 0x30;
    I2C_Init(I2C1,&I2C_InitStruct);
    I2C_Cmd(I2C1,ENABLE);
}

void STD_I2CDMACfg(void* DMA_CHx,unsigned int peripheralBaseAddr,unsigned int memoryBaseAddr,unsigned int bufferSize)
{
	DMA_InitTypeDef DMA_InitStruct;  					//DMA���� 
    NVIC_InitTypeDef NVIC_InitStruct;					//DMA�ж�����
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	//DMAʱ�ӿ���
    
	DMA_DeInit(DMA_CHx);								//Ĭ������ 
	
    DMA_InitStruct.DMA_BufferSize = bufferSize;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryBaseAddr = memoryBaseAddr;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_PeripheralBaseAddr = peripheralBaseAddr;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA_CHx,&DMA_InitStruct);
  
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;      //DMA�����ȼ�Ҫ��һЩ,�����Ӱ����Ļ����:I2C_GenerateSTOP(I2C1,ENABLE);���ܼ�ʱ�ص�I2C
                                                                //��Ϸ����ʾ:���ȼ�û�и��ڴ���.��������ʱ����ָ�:���û�и���.
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
    
    /*DMA�ж��¼�����*/
	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);
}

//�����趨Ϊ1025,������a-1
void STD_I2CDMAEnable(void* DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE );  			//�ر� DMA ��ָʾ��ͨ�� 	
 	DMA_SetCurrDataCounter(DMA_CHx,1025);	//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE); 				//ʹ�� DMA ��ָʾ��ͨ��
 
    I2C_DMACmd(I2C1,ENABLE);
}




