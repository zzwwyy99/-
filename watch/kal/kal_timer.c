#include "cal_timer.h" 

void KAL_TimerInit(void)
{    
	/* 对于裸机: 使用CAL */
	CAL_TimerInit();

	/* 对于rtthread: 使用软件定时器 */
	//rt_timer_create
}

