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
	g_tNormTime.hour = 1;		//慢8个小时,北京时间为东八区
	g_tNormTime.minutes = 0;
    g_tNormTime.second = 0;
	
	//UartSetNormalTime(&g_tNormTime); 	//通过串口更改时间   

	/* 将普通时间转换为时间戳 */
	RTCTimeStamp(&g_tNormTime);
	
	/* 开启rtc中断 */
	RtcNvicInit();
}

void rtc_test(void)
{
	RTCInit();

	while(1){
//		printf("当前年份为:%d.\r\n",g_tNormTime.year);
//		printf("当前月份为:%d.\r\n",g_tNormTime.month);
//		printf("当前日期为:%d.\r\n",g_tNormTime.day);
//		printf("当前时钟为:%d.\r\n",g_tNormTime.hour);
//		printf("当前分钟为:%d.\r\n",g_tNormTime.minutes);
//		printf("当前秒钟为:%d.\r\n",g_tNormTime.second);
//		printf("当前周几为:%d.\r\n",g_tNormTime.weekday);
        printf("%d:%d:%d:%d:%d:%d:%d.\r\n",g_tNormTime.year,g_tNormTime.month,g_tNormTime.day,g_tNormTime.hour,g_tNormTime.minutes,g_tNormTime.second,g_tNormTime.weekday);
        
		delay(10000);
	}
	
}

//消息数据
struct msg_topic g_tMsgRTC = {
	.name = "rtc_topic",
	.msg_data = &g_tNormTime
};

//发布者
void RTCPublish(void *arg)
{
	//唤醒消息中心线程
	CoreProcss(arg);
}

struct Publisher g_tPublisherrRTC = {
	.Publish = RTCPublish
};

void RTCThreadInit(void)
{
	printf("RTC Thread Init.\r\n");
    
    //注册MSG
    RegisterMessage("rtc_topic");
}

//RTC秒中断处理函数
void RTC_IRQHandler(void)
{	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){
		/* 清除rtc秒中断 */
		RTC_ClearITPendingBit(RTC_IT_SEC);

		/* 等待rtc写操作完成 */
		RTC_WaitForLastTask();

		/* 得到当前的时间戳,并转换为普通格式的时间 */
		RTCNormalTime(&g_tNormTime);
        
        //发送msg到业务子系统
        g_tPublisherrRTC.Publish(&g_tMsgRTC);
	}
}

