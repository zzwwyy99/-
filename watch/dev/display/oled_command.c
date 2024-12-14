#include <stm32f10x.h>
#include <stdio.h>
#include <rtthread.h>
#include <rthw.h>

#include "i2c_system.h"

#define	OLED_ADDR	0x78

//#define	OLEDInterfaceDev	I2CDevice
static I2CDevice *g_ptI2CDevice;

int OLED_I2C1_DMA_BUSYFLAG = 0;//DMA忙碌标志位,值为1:DMA正在传输数据,注意此时不能更改GRAM中的值

//在这里会用到一个I2CDevice,是在这里像i2c.c里面那样直接构造一个I2CDevice呢,还是GetI2CDevice("i2c1"),直接获得一个i2c设备
static int OLEDInfDevInit(void *frambuffer,int buffersize)
{
	/* 初始化i2c_system */
	ADDI2CDevice();
	
	/* 获得I2C1设备 */
	g_ptI2CDevice = GetI2CDevice("i2c1");
	g_ptI2CDevice->iDMAmemBaseAddr = (int)frambuffer;
	g_ptI2CDevice->iBufferSize = buffersize;
	
    g_ptI2CDevice->dam_cfg(g_ptI2CDevice);	//重新配置DMA
	return 0;
}

/**
* @brief  发送设备地址-发送要写入的寄存器addr-发送要写入寄存器的数据data
* @param  addr:写入数据的oled寄存器的地址
          data:写入oled寄存器addr的数据
* @retval 无
*/
//OLED i2c通信规定,先发送oled的地址,再发送oled寄存器的地址,再发送到oled寄存器的数据
static void OLEDByteSend(unsigned char reg_addr,unsigned char data) 
{
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));							//检测I2C是否繁忙
    
    I2C_GenerateSTART(I2C1,ENABLE);											//开启I2C,发送起始信号
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));				//事件5发生,表明已产生起始信号

    I2C_Send7bitAddress(I2C1,OLED_ADDR,I2C_Direction_Transmitter);			//发送Oled设备地址:0x78
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//事件6发生,表明找到地址为0x78的外设
    
    I2C_SendData(I2C1,reg_addr);												//I2C1发送addr到Oled
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));		//EV8
    
    I2C_SendData(I2C1,data);												//I2C1发送数据到Oled
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));			//EV8_2
    
    I2C_GenerateSTOP(I2C1,ENABLE);
}

/**
* @brief  发送数据到oled的0x00寄存器
* @param  cmd:0x00寄存的数据即命令
* @retval 无
*/
void OLEDCommandSend(unsigned char cmd)
{
    OLEDByteSend(0x00,cmd);
}

/**
* @brief  写命令
* @param  I2C_Data:数据
* @retval 无
*/
void OLEDDataSend(unsigned char data)
{
    OLEDByteSend(0x40,data);
}


/************** 1. 基础命令功能函数 **************/
/**
* @brief  设置Oled的光标位置
* @param  x:列位置,值为0~127
          y:行位置,值为0~7
* @retval 无
*/
void oled_setcursor(int x,int y)
{
    OLEDCommandSend(0xb0 | y);
    OLEDCommandSend(0x10 |(x & 0xF0) >> 4);		//取x的高四位
    OLEDCommandSend(0x00 |(x & 0x0F));			//取x的低四位
}

/************** 7. OLED初始化函数 **************/
/*oled屏幕初始化*/
void oled_init(char *pFramBuffer,int iSize)
{
    int i,j;
	
	/*初始化oled的IIC接口*/
	OLEDInfDevInit(pFramBuffer,iSize);
	
	/*上电延时*/
	for (i = 0; i < 1000; i++)		
	{
		for (j = 0; j < 1000; j++);
	}
	
	/*oled配置*/
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
	
	/*初始化IIC的DMA*/
    //g_ptI2CDevice->dam_cfg(g_ptI2CDevice);	//完成DMA配置
	//I2C_DMACmd(I2C1,ENABLE);				//开启DMA请求,这个请求只需要开启一次
}


/************** 8. 基本驱动功能函数 **************/
/**
* @brief  发送0x00到Oled
* @param  
* @retval 无
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
* @brief  将GRAM中的值更新到OLED中
* @param  uint8_t (*gram)[128]:刷新进oled的gram指针
* @retval 无
* @note   OLED设定为水平模式,只需要设定一次光标位置
		  一次只传输1个字节
*/
void oled_gram_refresh(unsigned char *gram)
{
	int i;
    
    oled_setcursor(0,0);
    for(i=0;i<1024;i++){
     	OLEDDataSend(*(gram + i));  //传输过程为IIC开始,传输一个字节,IIC关闭.
	}
}

/**
* @brief  非DMA传输.将GRAM中的值更新到OLED中
* @param  uint8_t (*gram)[128]:刷新进oled的gram指针
* @retval 无
* @note   OLED设定为水平模式,只需要设定一次光标位置
		  一次传输1024个字节
*/
void oled_gram_refresh_onetime(unsigned char *gram)
{
	int i;
	oled_setcursor(0,0);
	
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));							//检测I2C是否繁忙
    
    I2C_GenerateSTART(I2C1,ENABLE);											//开启I2C,发送起始信号
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));				//事件5发生,表明已产生起始信号
			
    I2C_Send7bitAddress(I2C1,OLED_ADDR,I2C_Direction_Transmitter);			//发送oled地址 0x78
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//事件6发生,表明找到地址为0x78的外设
    
    I2C_SendData(I2C1,0x40);												//将addr写入DR寄存器
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));		//EV8
 
	for(i=0;i<1024;i++){
     	I2C_SendData(I2C1,*(gram + i));										//将data地址写入DR寄存器
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));		//EV8_2
	}
	 
    I2C_GenerateSTOP(I2C1,ENABLE);
}

/**
* @brief  DMA传输,将GRAM中的值更新到OLED中
* @param  uint8_t (*gram)[128]:刷新进oled的gram指针
* @retval 无
* @note   OLED设定为水平模式,只需要设定一次光标位置
		  一次传输1024个字节,DMA传输
*/
extern int iFPS;
void oled_gram_refresh_dma(void)
{
    rt_interrupt_enter();
	if(OLED_I2C1_DMA_BUSYFLAG == 0){
        //printf("%dms刷新一次\r\n",iFPS*10);
        iFPS = 0;
        
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));							//检测I2C是否繁忙
    
		I2C_GenerateSTART(I2C1,ENABLE);											//开启I2C,发送起始信号
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));				//事件5发生,表明已产生起始信号
		
		I2C_Send7bitAddress(I2C1,OLED_ADDR,I2C_Direction_Transmitter);			//发送oled地址 0x78
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//事件6发生,表明找到地址为0x78的外设
		
		I2C_SendData(I2C1,0x40);												//将addr写入DR寄存器
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));		//EV8_2
    
        rt_interrupt_leave();

		/*打开DMA,允许DMA将数据从GRAM搬运到I2C->DR.*/
		g_ptI2CDevice->dam_enable(g_ptI2CDevice);

		OLED_I2C1_DMA_BUSYFLAG = 1;				//DMA忙标志位
	} 
}


//等待DMA传输完成，30ms左右.
//擦写显存禁止,或许可以改成信号量,这样申请信号量就可以不用死等.
void WaitForI2CDMABusy(void)
{
    while(OLED_I2C1_DMA_BUSYFLAG);
}

/**
* @brief  DMA传输完成中断处理函数
* @param  无
* @retval 无
*/
void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC6)){	//DMA传输完成
        DMA_ClearFlag(DMA1_FLAG_TC6);		//清除传输完成标志位
        
        //这个I2C_GenerateSTOP必须要有,否则I2C将一直是BUSY状态
        I2C_GenerateSTOP(I2C1,ENABLE);		//关闭I2C
 
        //printf("传输一次完成\r\n");       //放在I2C_GenerateSTOP()前面屏幕会向右滚动,和设定DMA_SetCurrDataCounter(DMA_CHx,1024);时也是向右滚动.\
                                                效果等同于多发送了一个字节给oled.向右滚动是多发送了一个字节,向左滚动是少发送了一个字节
 		/* 用信号量更合适 */
		OLED_I2C1_DMA_BUSYFLAG = 0;			//DMA忙碌标志位,值为1:DMA正在传输数据,此时没法再开启一次传输,但是注意此时不要更改GRAM中的值 
	}
}


