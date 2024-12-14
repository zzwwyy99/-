#include <stm32f10x.h>

//kal层,cal层直接省略

/* RTC中断配置:秒中断 */
void RtcNvicConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    
    NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3; //2bit,不能设置为4,否则的屏幕会右移.也就是DMA中断中没有及时STOP I2C.
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    
    NVIC_Init(&NVIC_InitStruct);
}

/* 配置RTC */
void RtcPeripheralConfig(void)
{
	//使能PWR和backup时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
	
	//允许访问Backup区域
	PWR_BackupAccessCmd(ENABLE);
	
	//复位Backup区域
	BKP_DeInit();
	
	/*使用内部时钟LSI*/
	RCC_LSICmd(ENABLE);
	
	//等待LSI准备好
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	//选择LSI作为RTC时钟源
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	//使能RTC时钟源
	RCC_RTCCLKCmd(ENABLE);
	
	//等待RTC时钟与APB时钟同步
	RTC_WaitForSynchro();
	
	//确保上一次RTC操作完成
	RTC_WaitForLastTask();
	
	//使能RTC秒中断
	RTC_ITConfig(RTC_IT_SEC,ENABLE);
	
	//确保上一次RTC操作完成
	RTC_WaitForLastTask();
	
	//进入配置模式
	RTC_EnterConfigMode();
	
	//设置RTC分频: 使 RTC 周期为 1s ,LSI 约为 40KHz
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */
	RTC_SetPrescaler(40000-1);
	
	//确保上一次RTC操作完成
	RTC_WaitForLastTask();	
	
	//退出配置模式
	RTC_ExitConfigMode();
}

