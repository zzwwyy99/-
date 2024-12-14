#ifndef __RTC_DEVICE_H
#define __RTC_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

#include "convert.h"

void RtcNvicInit(void);
void RTCTimeStamp(struct NormalTime * ptNormTime);
void RTCNormalTime(struct NormalTime * ptNormTime);
void UartSetNormalTime(struct NormalTime * ptNormTime);

#endif	/*__RTC_DEVICE_H */

