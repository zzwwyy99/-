#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "convert.h"

static int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  һ���ж���s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

/* ��ͨʱ���ʽתʱ��� */
int GetUnixTime(struct NormalTime * ptNormTime)
{
	if ((ptNormTime->month -= 2) <= 0) {		/* 1..12 -> 11,12,1..10 */
		ptNormTime->month += 12;				/* Puts Feb last since it has leap day */
		ptNormTime->year -= 1;
	}
	
	return ((((ptNormTime->year/4 - ptNormTime->year/100 + ptNormTime->year/400 + 367*ptNormTime->month/12 + ptNormTime->day) +
			ptNormTime->year*365 - 719499
	    )*24 + ptNormTime->hour 				/* now have hours */
	  )*60 + ptNormTime->minutes				/* now have minutes */
	)*60 + ptNormTime->second; 					/* finally seconds */
}


/*���㹫��*/
static void GetGregorianDay(struct NormalTime * ptNormTime)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear = ptNormTime->year - 1;

	/* ����ӹ�ԪԪ�굽������ǰһ��֮��һ�������˶��ٸ����� */
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

   	/* ������������һ��Ϊ���꣬�Ҽ������·���2��֮����������1�����򲻼�1 */
	if((ptNormTime->year % 4==0) &&((ptNormTime->year%100 != 0) || (ptNormTime->year % 400==0)) && (ptNormTime->month>2)){
		day = 1;
	} 
	else{
		day = 0;
	}

	/* ����ӹ�ԪԪ��Ԫ������������һ���ж����� */
	day += lastYear*365 + leapsToDate + MonthOffset[ptNormTime->month-1] + ptNormTime->day; 

	ptNormTime->weekday = day%7;
}

/* ʱ���ת��ͨʱ���ʽ */
void GetNormalTime(int iTimeStamp,struct NormalTime * ptNormTime)
{
	register int i;
	register long int hms, day;

	day = iTimeStamp / SECDAY;				/* �ж����� */
	hms = iTimeStamp % SECDAY;				/* �����ʱ�䣬��λs */

	ptNormTime->hour = hms / 3600;
	ptNormTime->minutes = (hms % 3600) / 60;
	ptNormTime->second = (hms % 3600) % 60;

	/*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	ptNormTime->year = i;

	/*���㵱ǰ���·�*/
	if (leapyear(ptNormTime->year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	ptNormTime->month = i;

	/*���㵱ǰ����*/
	ptNormTime->day = day + 1;

	GetGregorianDay(ptNormTime);	
}
