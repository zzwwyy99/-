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
	BUTTON_EVENT_CLICK_DOWN, /* �������������¼� 0*/
	BUTTON_EVENT_CLICK_UP, 	 /* �������������¼� 1*/
	BUTTON_EVENT_HOLD,       /* ����������ʼ�¼� 2*/
	BUTTON_EVENT_HOLD_CYC,   /* �������������Դ����¼� 3*/
	BUTTON_EVENT_HOLD_UP,    /* �������������¼� 4*/
	BUTTON_EVENT_NONE,       /* �ް����¼� 5*/
}my_button_envent;
	

/*struct ButtonDevice */
typedef struct Button{
	char *name;							/*��������*/ /*�豸��*/
	int  iPressLogicLevel; 				/*��������ʱ�ĵ�ƽ*/
	int  iCnt;				 			/*����ɨ�赽����״̬�Ĵ���*/
	int  iHoldCycPeriod ;				/*���������Իص�������*/
	int  iPin;							/*������Ӧ��pin���*/
	
	enum my_button_envent eEvent;		/*�����������¼�*/
	my_button_callback cb;				/*���������¼��Ļص����� */
}Button,*PButton;


PButton __GetButton(char *name);
void ButtonRegister(struct Button *pButton,char *name);
void __ButtonDevcieManagerInit(void);

#endif	/*__BUTTON_DEVICE_H*/

