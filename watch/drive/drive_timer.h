#ifndef __DRIVE_TIMER_H
#define __DRIVE_TIMER_H

#define DelayDeviceInit STD_Timer3Config

typedef void (*TimerProcessFunction)(void);

void STD_TimerConfig(void);
void SetTimerProcessFunction(TimerProcessFunction func);

void STD_Timer3Config(void);

#endif  /*__DRIVE_BUTTON_H*/

