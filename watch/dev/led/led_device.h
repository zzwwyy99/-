#ifndef __LED_DEVICE_H
#define __LED_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

#define	LED_ON	0
#define	LED_OFF	1
#define	LED_TROGGLE	2

/* button和LED设备都可以用一个设备接口,父类*/
/* 设备接口 */
typedef struct LedDevice{
	char *name;			/* 设备名 */
	int  iPin;			/* 对应的pin编号 */
	void *iPort; 		/* 对应的port编号 */
		
	void (*init)(struct LedDevice *ptLedDevice);						/* 设备初始化函数 */
	void (*control)(struct LedDevice *ptLedDevice,int istatus);			/* 设备控制函数,istatus:on, off, toggle */
	void (*set)(struct LedDevice *ptLedDevice,int iset);				/* 设备设定函数 */
}LedDevice,*PLedDevice;

void LedDeviceRegister(struct LedDevice *ptLedDevice,char *name);
PLedDevice __GetLedDevice(char *name);

#endif	/*__LED_DEVICE_H*/

