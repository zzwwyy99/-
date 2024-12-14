#include <stdio.h>

#include "display_device.h"
#include "oled.h"
#include "display_system.h"

PDisplayDevice g_ptFaultDisplayDev;

void ADDDisplayDevice(void)
{
	/* ���Display�豸��DisplayDevice�豸������ */
	AddDisplayDeviceOLED();
}

/*���һ��DisplayDevice*/
PDisplayDevice GetDisplayDevice(char *name)
{
	return __GetDisplayDevice(name);
}

//�趨һ��Ĭ��DisplayDevice
PDisplayDevice SetDefaultDisplayDev(void)
{
	/* �����ʾ�豸�������� */
	ADDDisplayDevice();
	
	g_ptFaultDisplayDev = GetDisplayDevice("oled");

	if(g_ptFaultDisplayDev){
		printf("����Ĭ��DisplayDevice��OLED�ɹ�.\r\n");
		return g_ptFaultDisplayDev;
	}else{
		printf("����Ĭ��DisplayDevice��OLEDʧ��.\r\n");
		return 0;
	}
}

int iFPS = 0;
//��ʱˢ��
int TimerFlushDisplayDev(void)
{  
	 iFPS++;
	 
     g_ptFaultDisplayDev->flush(g_ptFaultDisplayDev);

	 return 0;
}

//��ʱ��ˢ����ʾ�豸��ʼ
void TimerFlushDisplayDevControl(int arg)
{  
	__DisplayDevTimerFlushControl(arg);
}

