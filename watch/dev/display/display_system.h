#ifndef __DISPLAY_SYSTEM_H
#define __DISPLAY_SYSTEM_H

#include "display_device.h"
                    
#define TIMER_FLUSH(a) if(a) __DisplayDevTimerFlushControl(TIMER_FLUSH_CMD_ENABLE);\
                            else __DisplayDevTimerFlushControl(TIMER_FLUSH_CMD_DISABLE);
                            
void ADDDisplayDevice(void);
PDisplayDevice GetDisplayDevice(char *name);
PDisplayDevice SetDefaultDisplayDev(void);
int TimerFlushDisplayDev(void);
void TimerFlushDisplayDevStart(void);

#endif	/*__DISPLAY_SYSTEM_H*/


