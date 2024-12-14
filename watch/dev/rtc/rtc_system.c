#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "drive_delay.h"
#include "drive_rtc.h"
#include "rtc_device.h"
#include "rtc_system.h"
#include "convert.h"
#include "message_manager.h"

static struct NormalTime g_tNormTime;

void RTCInit(void)
{		
	g_tNormTime.year = 2024;
	g_tNormTime.month = 1;
	g_tNormTime.day = 1;
	g_tNormTime.hour = 1;		//��8��Сʱ,����ʱ��Ϊ������
	g_tNormTime.minutes = 0;
    g_tNormTime.second = 0;
	
	//UartSetNormalTime(&g_tNormTime); 	//ͨ�����ڸ���ʱ��   

	/* ����ͨʱ��ת��Ϊʱ��� */
	RTCTimeStamp(&g_tNormTime);
	
	/* ����rtc�ж� */
	RtcNvicInit();
}

void rtc_test(void)
{
	RTCInit();

	while(1){
//		printf("��ǰ���Ϊ:%d.\r\n",g_tNormTime.year);
//		printf("��ǰ�·�Ϊ:%d.\r\n",g_tNormTime.month);
//		printf("��ǰ����Ϊ:%d.\r\n",g_tNormTime.day);
//		printf("��ǰʱ��Ϊ:%d.\r\n",g_tNormTime.hour);
//		printf("��ǰ����Ϊ:%d.\r\n",g_tNormTime.minutes);
//		printf("��ǰ����Ϊ:%d.\r\n",g_tNormTime.second);
//		printf("��ǰ�ܼ�Ϊ:%d.\r\n",g_tNormTime.weekday);
        printf("%d:%d:%d:%d:%d:%d:%d.\r\n",g_tNormTime.year,g_tNormTime.month,g_tNormTime.day,g_tNormTime.hour,g_tNormTime.minutes,g_tNormTime.second,g_tNormTime.weekday);
        
		delay(10000);
	}
	
}

//��Ϣ����
struct msg_topic g_tMsgRTC = {
	.name = "rtc_topic",
	.msg_data = &g_tNormTime
};

//������
void RTCPublish(void *arg)
{
	//������Ϣ�����߳�
	CoreProcss(arg);
}

struct Publisher g_tPublisherrRTC = {
	.Publish = RTCPublish
};

void RTCThreadInit(void)
{
	printf("RTC Thread Init.\r\n");
    
    //ע��MSG
    RegisterMessage("rtc_topic");
}

//RTC���жϴ�����
void RTC_IRQHandler(void)
{	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){
		/* ���rtc���ж� */
		RTC_ClearITPendingBit(RTC_IT_SEC);

		/* �ȴ�rtcд������� */
		RTC_WaitForLastTask();

		/* �õ���ǰ��ʱ���,��ת��Ϊ��ͨ��ʽ��ʱ�� */
		RTCNormalTime(&g_tNormTime);
        
        //����msg��ҵ����ϵͳ
        g_tPublisherrRTC.Publish(&g_tMsgRTC);
	}
}

