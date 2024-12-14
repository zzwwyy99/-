#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"

#include "convert.h"

static int month_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  一天有多少s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])

/* 普通时间格式转时间戳 */
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


/*计算公历*/
static void GetGregorianDay(struct NormalTime * ptNormTime)
{
	int leapsToDate;
	int lastYear;
	int day;
	int MonthOffset[] = { 0,31,59,90,120,151,181,212,243,273,304,334 };

	lastYear = ptNormTime->year - 1;

	/* 计算从公元元年到计数的前一年之中一共经历了多少个闰年 */
	leapsToDate = lastYear/4 - lastYear/100 + lastYear/400;      

   	/* 如若计数的这一年为闰年，且计数的月份在2月之后，则日数加1，否则不加1 */
	if((ptNormTime->year % 4==0) &&((ptNormTime->year%100 != 0) || (ptNormTime->year % 400==0)) && (ptNormTime->month>2)){
		day = 1;
	} 
	else{
		day = 0;
	}

	/* 计算从公元元年元旦到计数日期一共有多少天 */
	day += lastYear*365 + leapsToDate + MonthOffset[ptNormTime->month-1] + ptNormTime->day; 

	ptNormTime->weekday = day%7;
}

/* 时间戳转普通时间格式 */
void GetNormalTime(int iTimeStamp,struct NormalTime * ptNormTime)
{
	register int i;
	register long int hms, day;

	day = iTimeStamp / SECDAY;				/* 有多少天 */
	hms = iTimeStamp % SECDAY;				/* 今天的时间，单位s */

	ptNormTime->hour = hms / 3600;
	ptNormTime->minutes = (hms % 3600) / 60;
	ptNormTime->second = (hms % 3600) % 60;

	/*算出当前年份，起始的计数年份为1970年*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	ptNormTime->year = i;

	/*计算当前的月份*/
	if (leapyear(ptNormTime->year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	ptNormTime->month = i;

	/*计算当前日期*/
	ptNormTime->day = day + 1;

	GetGregorianDay(ptNormTime);	
}
