#ifndef __TIME_CONVERT_H
#define __TIME_CONVERT_H

//标准时间结构体
typedef struct NormalTime{
	int second;
	int minutes;
	int hour;
	int day;
	int month;
	int year;
	int weekday;
}NormalTime;

int GetUnixTime(struct NormalTime * ptNormTime);
void GetNormalTime(int iTimeStamp,struct NormalTime * ptNormTime);

#endif	/*__TIME_CONVERT_H*/

