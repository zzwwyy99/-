#include <string.h>

#include "input_device.h"

//�����豸������
static InputDevice *g_ptInputDeviceManager;

//�����豸ע��
void InputDeviceRegister(PInputDevice pInputDevice)
{
	pInputDevice->pNext = g_ptInputDeviceManager;
	g_ptInputDeviceManager = pInputDevice;
}

//���豸���������һ�������豸
PInputDevice __GetInputDevice(char *name)
{
	PInputDevice ptemp;
	ptemp = g_ptInputDeviceManager;
	
	while(ptemp){
		if(strcmp(ptemp->name,name) == 0){
			return ptemp;
		}else{
			ptemp = ptemp->pNext;
		}
	}

	return NULL;
}

/* �����豸��ʼ�� */
void __InitInputDevice(void)
{
	PInputDevice ptemp;
	ptemp = g_ptInputDeviceManager;
	
	while(ptemp){
        ptemp->DevceInit();  //ButtonDev��ButtonSystem���Ѿ���ʼ��
        ptemp = ptemp->pNext;
	}
}
