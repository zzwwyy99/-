#ifndef __BEEP_DEVICE_H
#define __BEEP_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

/*struct BeepDevice */
//���ﻹ������PIN��PORT,�����RT-Thread�Ļ�,ֻ��Ҫ����һ��pin(���).
typedef struct BeepDevice{
	char *name;  		/* �豸�� */
	int  iPin;			/* ��ʱ����ͨ������pin:��ʱ��4,CH3,PB8 */
	void *iPort; 		/* GPIO�Ķ˿�port��� */
	void *timer;  		/* BEEP����timer��ʱ�����*/
	
    void (*init)(struct BeepDevice *ptBeepDevice);    	/* �豸��ʼ������ */    
	void (*pwm_cfg)(struct BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse);		
	void (*pwm_on)(struct BeepDevice *ptBeepDevice);
	void (*pwm_off)(struct BeepDevice *ptBeepDevice);
}BeepDevice,*PBeepDevice;

PBeepDevice __GetBeepDevice(char *name);
void BeepDeviceRegister(struct BeepDevice *ptBeepDevice,char *name);


#endif	/*__BEEP_DEVICE_H*/

