#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "drive_rtc.h"
#include "rtc_device.h"
#include "convert.h"

//RTC中断初始化
void RtcNvicInit(void)
{	
	RtcNvicConfig();
}

/* 标准时间转换为时间戳(UNIX time),并保存到RTC中 */
void RTCTimeStamp(struct NormalTime *ptNormTime)
{	
	//RTC配置
	RtcPeripheralConfig();
	
	//确保上一次RTC操作完成
	RTC_WaitForLastTask();

	//计算时间戳并写入RTC寄存器
	RTC_SetCounter(GetUnixTime(ptNormTime));
    
	//确保上一次RTC操作完成
	RTC_WaitForLastTask();
}


/* 当前时间戳转换为标准时间 */
void RTCNormalTime(struct NormalTime *ptNormTime)
{
	int TimeVal;

	//得到时间戳
	TimeVal = RTC_GetCounter();
	GetNormalTime(TimeVal,ptNormTime);
}


/*
 * 函数名：UartSetNormalTime
 * 描述  ：保存用户使用串口设置的时间，
 *         以便后面转化成时间戳存储到RTC计数寄存器中。
 * 输入  ：用于读取RTC时间的结构体指针
 * 注意  ：在串口调试助手输入时，输入完数字要加回车
 */
void UartSetNormalTime(struct NormalTime *ptNormTime)
{
	int number = 0;
	int max_day = 0;

	printf("=========================设置时间==================\r\n");

	do
	{
		printf("输入年份(Please Set Years),范围[1970~2038],输入字符后请加回车.\r\n");
		scanf("%d",&number);
		if(number <1970 || number >2038)
		{
			printf("输入的数字是:%d,不符合要求.\n\r",number);
		}else{	  
			printf("年份被设置为:%d\n\r", number);
			ptNormTime->year = number;
			break;
		}
	}while(1);

	do 
	{
		printf("输入月份(Please Set Months):范围[1~12],输入字符后请加回车.\r\n");
		scanf("%d",&number);
		if(number < 1 || number >12)
		{
			printf("输入的数字是:%d,不符合要求.\r\n",number); 
		}else{	  
			printf("月份被设置为:%d\n\r", number);
			ptNormTime->month = number;
			break;
		}
	}while(1);

	/*根据月份计算最大日期*/
	switch(ptNormTime->month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:					
			max_day = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			max_day = 30;
			break;
		
		case 2:					
		    /*计算闰年*/
			if((ptNormTime->year%4==0) && ((ptNormTime->year%100!=0) || (ptNormTime->year%400==0)) && (ptNormTime->month > 2)){
				max_day = 29;
			}else{
				max_day = 28;
			}
			break;			
	}

	do 
	{				
		printf("输入日期(Please Set Day),范围[1~%d],输入字符后请加回车.\r\n",max_day);
		scanf("%d",&number);
		if(number < 1 || number > max_day){
			printf("输入的数字是:%d,不符合要求.\r\n",number);
		}else{
			printf("日期被设置为:%d\r\n", number);
			ptNormTime->day = number;
			break;
		}
	}while(1);

	do 
	{				
		printf("输入时钟(Please Set Hours),范围[0~23],输入字符后请加回车.\r\n");
		scanf("%d",&number);

		if(number >23){
			printf("输入的数字是:%d,不符合要求.\r\n",number);
		}else{		
			printf("时钟被设置为:%d\r\n", number);

			ptNormTime->hour = number;
			break;
		}
	}while(1);

	do 
	{				
		printf("输入分钟(Please Set Minutes),范围[0~59]，输入字符后请加回车.\r\n");
		scanf("%d",&number);

		if( number >59){
			printf("输入的数字是:%d,不符合要求.\r\n",number);
		}else{
			printf("分钟被设置为:%d\r\n", number);

			ptNormTime->minutes = number;
			break;
		}
	}while(1);

	do 
	{				
		printf("输入秒钟(Please Set Seconds),范围[0~59],输入字符后请加回车.\r\n");
		scanf("%d",&number);

		if( number >59){
			printf("输入的数字是:%d,不符合要求.\r\n",number);
		}else{
			printf("秒钟被设置为:%d\r\n", number);
			
			ptNormTime->second = number;
			break;
		}
	}while(1);
}

