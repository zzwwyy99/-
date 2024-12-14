#ifndef __DHT11_DEVICE_H
#define __DHT11_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

void DHT11Init(void);
int DHT11ReceiveData(char *data);

#endif	/*__DHT11_DEVICE_H*/

