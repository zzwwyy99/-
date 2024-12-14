#include "drive_i2c.h"

/**
* @brief  初始化I2C1的GPIO引脚,配置I2C1
* @param  无
* @retval 无
*/
void STD_GPIOI2CDevConfig(void *port,int pin1,int pin2) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;
    
    //开启I2C1及GPIOB时钟
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
	DMA_InitTypeDef DMA_InitStruct;  					//DMA配置 
    NVIC_InitTypeDef NVIC_InitStruct;					//DMA中断配置
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	//DMA时钟开启
    
	DMA_DeInit(DMA_CHx);								//默认配置 
	
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
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;      //DMA的优先级要高一些,否则会影响屏幕右移:I2C_GenerateSTOP(I2C1,ENABLE);不能及时关掉I2C
                                                                //游戏的显示:优先级没有高于串口.恐龙跳的时候的手感:这个没有复现.
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
    
    /*DMA中断事件配置*/
	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);
}

//必须设定为1025,可能是a-1
void STD_I2CDMAEnable(void* DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE );  			//关闭 DMA 所指示的通道 	
 	DMA_SetCurrDataCounter(DMA_CHx,1025);	//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE); 				//使能 DMA 所指示的通道
 
    I2C_DMACmd(I2C1,ENABLE);
}




