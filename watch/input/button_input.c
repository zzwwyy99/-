#include <drive_timer.h>
#include <stdio.h>
#include <rtthread.h>

#include "button.h"
#include "input_device.h"
#include "input_buffer.h"

extern struct rt_thread g_tLogicThread;

/* �����ص����� */
static void ButtonInputDeviceCallBack(void *arg)
{
	PButton ptemp = (PButton)arg;
	InputEvent event;
	
	event.etype = INPUT_EVENT_TYPE_BUTTON;
	/* switch����ж����ĸ����� */
    switch(ptemp->iPin){
        case 64:
            event.iKey = 1;
            break;
        case 2:
            event.iKey = 2;
            break;
        case 256:
            event.iKey = 3;
            break;
        default:
            break;
    }
	event.iPressure = ptemp->eEvent;

	/* ���뻷�λ�����*/
	PutInputBuffer(&event);
}

//�ö�ʱ��2ɨ��.������Ҫ��ʼ����ʱ��2
//��ʱ�����������ʼ��,��ӵ�����������������
static int ButtonInputInit(void)
{
	/* Button�豸,ɨ�谴���õ���timer,����button_system�г�ʼ�� */

	/* �趨�����Ļص�����:button_cb */
	SetButtonInputDeviceCallBack(ButtonInputDeviceCallBack);
    
    return 0;
}

//����һ��button�����豸
static InputDevice g_tButtonInputDevice = {
	"button",
	NULL,
	ButtonInputInit,
	NULL
};

//��button�豸ע�ᵽ�豸������
void AddButtonInputDevice(void)
{
	InputDeviceRegister(&g_tButtonInputDevice);
}

