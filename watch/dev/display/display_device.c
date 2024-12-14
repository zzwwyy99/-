#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "display_device.h"

#define DISPLAY_DEVICE_LIST_MAX	2

/* ����һ��DisplayDevice������ */
typedef struct DisplayDeviceManager{
	unsigned int num;

	//�����buttonDevһ��,����Ҫ��һ����ʱ��ˢ��,���Ծ͹���һ����,�����������ʱ��,��Ҫ�����ö�ʱ��
	//void (*DisplayDevTimerInit)(void);  /* ��ʱˢ���õ��Ķ�ʱ�� */

	//������ʱ����ʱˢ��
	void (*TimerFlushControl)(int arg); 
	
	struct DisplayDevice *display_device_list[DISPLAY_DEVICE_LIST_MAX];
}DisplayDeviceManager;

int flush_start = 0;
void DisplayDevTimerFlushControl(int arg)
{
    int cmd = arg;
    
	//֡�ʲ���
    if(cmd == TIMER_FLUSH_CMD_ENABLE){
        //printf("��ʱ��ˢ����Ļ����.\r\n");
        flush_start = 1;	//һ����־λ,�����ź���
    }else{
        //printf("��ʱ��ˢ����Ļ�ر�.\r\n");
        flush_start = 0;	//һ����־λ,�����ź���
    }    
}

//DisplayDevice������
static DisplayDeviceManager g_tDisplayDeviceManager ={
	.num = 0,
	.TimerFlushControl = DisplayDevTimerFlushControl
};

void __DisplayDevTimerFlushControl(int arg)
{
	g_tDisplayDeviceManager.TimerFlushControl(arg);
}

/*ע�ắ��*/
void DisplayDeviceRegister(struct DisplayDevice *ptDisplayDevice,char *name)
{
	/* ��ʼ��DisplayDevice */
	ptDisplayDevice->init(ptDisplayDevice);
	ptDisplayDevice->name = name;
	
	/*  ���DisplayDevice��DisplayDevice�豸������ */
	g_tDisplayDeviceManager.display_device_list[g_tDisplayDeviceManager.num++] = ptDisplayDevice;
}

/*���dev*/
PDisplayDevice __GetDisplayDevice(char *name)
{
	PDisplayDevice ptDisplayDevice;

	for(int i=0;i<g_tDisplayDeviceManager.num;i++){
		ptDisplayDevice = g_tDisplayDeviceManager.display_device_list[i];
		if(strcmp(ptDisplayDevice->name,name) == 0){
			return ptDisplayDevice;			
		}
	}

	return NULL;
}




