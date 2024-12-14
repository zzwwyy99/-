#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "drive_rtc.h"
#include "rtc_device.h"
#include "convert.h"

//RTC�жϳ�ʼ��
void RtcNvicInit(void)
{	
	RtcNvicConfig();
}

/* ��׼ʱ��ת��Ϊʱ���(UNIX time),�����浽RTC�� */
void RTCTimeStamp(struct NormalTime *ptNormTime)
{	
	//RTC����
	RtcPeripheralConfig();
	
	//ȷ����һ��RTC�������
	RTC_WaitForLastTask();

	//����ʱ�����д��RTC�Ĵ���
	RTC_SetCounter(GetUnixTime(ptNormTime));
    
	//ȷ����һ��RTC�������
	RTC_WaitForLastTask();
}


/* ��ǰʱ���ת��Ϊ��׼ʱ�� */
void RTCNormalTime(struct NormalTime *ptNormTime)
{
	int TimeVal;

	//�õ�ʱ���
	TimeVal = RTC_GetCounter();
	GetNormalTime(TimeVal,ptNormTime);
}


/*
 * ��������UartSetNormalTime
 * ����  �������û�ʹ�ô������õ�ʱ�䣬
 *         �Ա����ת����ʱ����洢��RTC�����Ĵ����С�
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ע��  ���ڴ��ڵ�����������ʱ������������Ҫ�ӻس�
 */
void UartSetNormalTime(struct NormalTime *ptNormTime)
{
	int number = 0;
	int max_day = 0;

	printf("=========================����ʱ��==================\r\n");

	do
	{
		printf("�������(Please Set Years),��Χ[1970~2038],�����ַ�����ӻس�.\r\n");
		scanf("%d",&number);
		if(number <1970 || number >2038)
		{
			printf("�����������:%d,������Ҫ��.\n\r",number);
		}else{	  
			printf("��ݱ�����Ϊ:%d\n\r", number);
			ptNormTime->year = number;
			break;
		}
	}while(1);

	do 
	{
		printf("�����·�(Please Set Months):��Χ[1~12],�����ַ�����ӻس�.\r\n");
		scanf("%d",&number);
		if(number < 1 || number >12)
		{
			printf("�����������:%d,������Ҫ��.\r\n",number); 
		}else{	  
			printf("�·ݱ�����Ϊ:%d\n\r", number);
			ptNormTime->month = number;
			break;
		}
	}while(1);

	/*�����·ݼ����������*/
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
		    /*��������*/
			if((ptNormTime->year%4==0) && ((ptNormTime->year%100!=0) || (ptNormTime->year%400==0)) && (ptNormTime->month > 2)){
				max_day = 29;
			}else{
				max_day = 28;
			}
			break;			
	}

	do 
	{				
		printf("��������(Please Set Day),��Χ[1~%d],�����ַ�����ӻس�.\r\n",max_day);
		scanf("%d",&number);
		if(number < 1 || number > max_day){
			printf("�����������:%d,������Ҫ��.\r\n",number);
		}else{
			printf("���ڱ�����Ϊ:%d\r\n", number);
			ptNormTime->day = number;
			break;
		}
	}while(1);

	do 
	{				
		printf("����ʱ��(Please Set Hours),��Χ[0~23],�����ַ�����ӻس�.\r\n");
		scanf("%d",&number);

		if(number >23){
			printf("�����������:%d,������Ҫ��.\r\n",number);
		}else{		
			printf("ʱ�ӱ�����Ϊ:%d\r\n", number);

			ptNormTime->hour = number;
			break;
		}
	}while(1);

	do 
	{				
		printf("�������(Please Set Minutes),��Χ[0~59]�������ַ�����ӻس�.\r\n");
		scanf("%d",&number);

		if( number >59){
			printf("�����������:%d,������Ҫ��.\r\n",number);
		}else{
			printf("���ӱ�����Ϊ:%d\r\n", number);

			ptNormTime->minutes = number;
			break;
		}
	}while(1);

	do 
	{				
		printf("��������(Please Set Seconds),��Χ[0~59],�����ַ�����ӻس�.\r\n");
		scanf("%d",&number);

		if( number >59){
			printf("�����������:%d,������Ҫ��.\r\n",number);
		}else{
			printf("���ӱ�����Ϊ:%d\r\n", number);
			
			ptNormTime->second = number;
			break;
		}
	}while(1);
}

