#ifndef __LED_DEVICE_H
#define __LED_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

#define	LED_ON	0
#define	LED_OFF	1
#define	LED_TROGGLE	2

/* button��LED�豸��������һ���豸�ӿ�,����*/
/* �豸�ӿ� */
typedef struct LedDevice{
	char *name;			/* �豸�� */
	int  iPin;			/* ��Ӧ��pin��� */
	void *iPort; 		/* ��Ӧ��port��� */
		
	void (*init)(struct LedDevice *ptLedDevice);						/* �豸��ʼ������ */
	void (*control)(struct LedDevice *ptLedDevice,int istatus);			/* �豸���ƺ���,istatus:on, off, toggle */
	void (*set)(struct LedDevice *ptLedDevice,int iset);				/* �豸�趨���� */
}LedDevice,*PLedDevice;

void LedDeviceRegister(struct LedDevice *ptLedDevice,char *name);
PLedDevice __GetLedDevice(char *name);

#endif	/*__LED_DEVICE_H*/

