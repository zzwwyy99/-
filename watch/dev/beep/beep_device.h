#ifndef __BEEP_DEVICE_H
#define __BEEP_DEVICE_H

#ifndef NULL
#define NULL (void *)0
#endif

/*struct BeepDevice */
//这里还保存了PIN和PORT,如果用RT-Thread的话,只需要保存一个pin(编号).
typedef struct BeepDevice{
	char *name;  		/* 设备名 */
	int  iPin;			/* 定时器的通道所用pin:定时器4,CH3,PB8 */
	void *iPort; 		/* GPIO的端口port编号 */
	void *timer;  		/* BEEP所用timer定时器编号*/
	
    void (*init)(struct BeepDevice *ptBeepDevice);    	/* 设备初始化函数 */    
	void (*pwm_cfg)(struct BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse);		
	void (*pwm_on)(struct BeepDevice *ptBeepDevice);
	void (*pwm_off)(struct BeepDevice *ptBeepDevice);
}BeepDevice,*PBeepDevice;

PBeepDevice __GetBeepDevice(char *name);
void BeepDeviceRegister(struct BeepDevice *ptBeepDevice,char *name);


#endif	/*__BEEP_DEVICE_H*/

