#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "beep_device.h"

#define BEEP_DEVICE_LIST_MAX	2

//��һ���൱���豸������ܲ�

/* ����һ��BeepDevice������ */
typedef struct BeepDeviceManager{
	unsigned int num;
	struct BeepDevice *beep_device_list[BEEP_DEVICE_LIST_MAX];
}BeepDeviceManager;

//BeepDevice������
static BeepDeviceManager g_tBeepDeviceManager;

/*ע�ắ��*/
void BeepDeviceRegister(struct BeepDevice *ptBeepDevice,char *name)
{
	/* ��ʼ��BeepDevice */
	ptBeepDevice->init(ptBeepDevice);
	ptBeepDevice->name = name;
	printf("ע��BeepDevice:%s�ɹ�.\r\n",name);
	
	/*  ���BeepDevice��BeepDevice������ */
	g_tBeepDeviceManager.beep_device_list[g_tBeepDeviceManager.num++] = ptBeepDevice;
}

/*���һ���豸*/
PBeepDevice __GetBeepDevice(char *name)
{
	PBeepDevice ptBeepDevice;

	for(int i=0;i<g_tBeepDeviceManager.num;i++){
		ptBeepDevice = g_tBeepDeviceManager.beep_device_list[i];
		if(strcmp(ptBeepDevice->name,name) == 0){
            printf("�õ��豸:%s\r\n",name);
            
			return ptBeepDevice;			
		}
	}
    printf("û�еõ��豸:%s\r\n",name);
    
	return NULL;
}
