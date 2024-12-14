#ifndef __BUTTON_DEVICE_H
#define __BUTTON_DEVICE_H

#include "stm32f10x.h"

#ifndef NULL
#define NULL (void *)0
#endif

#define MY_BUTTON_DOWN_MS	50
#define MY_BUTTON_HOLD_MS	700

#define MY_BUTTON_SCAN_SPACE_MS	 20
#define MY_BUTTON_LIST_MAX	10

typedef void(*my_button_callback)(void *);

typedef enum my_button_envent{
	BUTTON_EVENT_CLICK_DOWN, /* 按键单击按下事件 0*/
	BUTTON_EVENT_CLICK_UP, 	 /* 按键单击结束事件 1*/
	BUTTON_EVENT_HOLD,       /* 按键长按开始事件 2*/
	BUTTON_EVENT_HOLD_CYC,   /* 按键长按周期性触发事件 3*/
	BUTTON_EVENT_HOLD_UP,    /* 按键长按结束事件 4*/
	BUTTON_EVENT_NONE,       /* 无按键事件 5*/
}my_button_envent;
	

/*struct ButtonDevice */
typedef struct Button{
	char *name;							/*按键名称*/ /*设备名*/
	int  iPressLogicLevel; 				/*按键按下时的电平*/
	int  iCnt;				 			/*连续扫描到按下状态的次数*/
	int  iHoldCycPeriod ;				/*长按周期性回调的周期*/
	int  iPin;							/*按键对应的pin编号*/
	
	enum my_button_envent eEvent;		/*按键触发的事件*/
	my_button_callback cb;				/*按键触发事件的回调函数 */
}Button,*PButton;


PButton __GetButton(char *name);
void ButtonRegister(struct Button *pButton,char *name);
void __ButtonDevcieManagerInit(void);

#endif	/*__BUTTON_DEVICE_H*/

