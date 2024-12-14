#ifndef _INPUT_DEVICE_H
#define _INPUT_DEVICE_H

#define INPUT_BUFF_LEN 20

typedef enum{
	INPUT_EVENT_TYPE_KEY,			/*外部中断实现的按键*/				
	INPUT_EVENT_TYPE_BUTTON,		/*定时器中断实现的按键*/
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
	int iPressure;					/*类型为INPUT_EVENT_TYPE_BUTTON时,记录Button产生的事件*/
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



