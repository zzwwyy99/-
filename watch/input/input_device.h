#ifndef _INPUT_DEVICE_H
#define _INPUT_DEVICE_H

#define INPUT_BUFF_LEN 20

typedef enum{
	INPUT_EVENT_TYPE_KEY,			/*�ⲿ�ж�ʵ�ֵİ���*/				
	INPUT_EVENT_TYPE_BUTTON,		/*��ʱ���ж�ʵ�ֵİ���*/
	INPUT_EVENT_TYPE_TOUCH,
	INPUT_EVENT_TYPE_NET,
	INPUT_EVENT_TYPE_STDIO,
}INPUT_EVENT_TYPE;

typedef struct InputEvent{
	int time;
	INPUT_EVENT_TYPE etype;
	int iX;
	int iY;	
	int iKey;
	int iPressure;					/*����ΪINPUT_EVENT_TYPE_BUTTONʱ,��¼Button�������¼�*/
	char str[INPUT_BUFF_LEN];
}InputEvent,*PInputEvent;

typedef struct InputDevice{
	char *name;
	int (*GetInputEvent)(PInputEvent ptInputEvent);
	int (*DevceInit)(void);	
	int (*DevceExit)(void);
	struct InputDevice *pNext;
}InputDevice,*PInputDevice;

void InputDeviceRegister(PInputDevice pInputDevice);
PInputDevice __GetInputDevice(char *name);
void __InitInputDevice(void);

#endif /*_INPUT_DEVICE_H*/



