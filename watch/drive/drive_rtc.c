#include <stm32f10x.h>

//kal��,cal��ֱ��ʡ��

/* RTC�ж�����:���ж� */
void RtcNvicConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    
    NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3; //2bit,��������Ϊ4,�������Ļ������.Ҳ����DMA�ж���û�м�ʱSTOP I2C.
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
    
    NVIC_Init(&NVIC_InitStruct);
}

/* ����RTC */
void RtcPeripheralConfig(void)
{
	//ʹ��PWR��backupʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
	
	//�������Backup����
	PWR_BackupAccessCmd(ENABLE);
	
	//��λBackup����
	BKP_DeInit();
	
	/*ʹ���ڲ�ʱ��LSI*/
	RCC_LSICmd(ENABLE);
	
	//�ȴ�LSI׼����
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	//ѡ��LSI��ΪRTCʱ��Դ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	//ʹ��RTCʱ��Դ
	RCC_RTCCLKCmd(ENABLE);
	
	//�ȴ�RTCʱ����APBʱ��ͬ��
	RTC_WaitForSynchro();
	
	//ȷ����һ��RTC�������
	RTC_WaitForLastTask();
	
	//ʹ��RTC���ж�
	RTC_ITConfig(RTC_IT_SEC,ENABLE);
	
	//ȷ����һ��RTC�������
	RTC_WaitForLastTask();
	
	//��������ģʽ
	RTC_EnterConfigMode();
	
	//����RTC��Ƶ: ʹ RTC ����Ϊ 1s ,LSI ԼΪ 40KHz
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */
	RTC_SetPrescaler(40000-1);
	
	//ȷ����һ��RTC�������
	RTC_WaitForLastTask();	
	
	//�˳�����ģʽ
	RTC_ExitConfigMode();
}

