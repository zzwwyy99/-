#include <stdlib.h>
#include <string.h>

#include <kal_button.h>
#include <kal_timer.h>

#include "button_device.h"
#include "drive_timer.h"

#define MY_BUTTON_LIST_MAX	10

#define MY_BUTTON_CALL(func,argv) \
		   do{if((func) != 0) func argv;}while(0)
           
//������������
//�����ʱ��������ȥ,����ǰ���rtt�ļӵ�����ȷʵ����õ�,�������豸���߷ŵ������ṹ������ȷʵ��̫��
typedef struct ButtonDevcieManager{
	unsigned int num;               /* ��ע��İ�����Ŀ */
    void (*ButtonTimerInit)(void);  /* ����ɨ���õ��Ķ�ʱ�� */
	struct Button *button_list[MY_BUTTON_LIST_MAX];     /* �洢����ָ�������*/
}ButtonDevcieManager;

static struct ButtonDevcieManager g_tButtonDevcieManager;

/* ��ʱ��Ӳ����ʼ�� */
static void ButtonTimerInit(void)
{
    KAL_TimerInit();
}

//����ģʽ
//��������:ʵ��һ������������
static ButtonDevcieManager *ConstrcutorButtonManage(void)
{
	static ButtonDevcieManager *ptManage;
	
	if(!ptManage){
		/* ���� */
		if(!ptManage){
			ptManage = (ButtonDevcieManager *)malloc(sizeof(struct ButtonDevcieManager));
			ptManage->num = 0;
            ptManage->ButtonTimerInit = ButtonTimerInit;
		}
		/* ���� */
	}
	return ptManage;
}


/* ��ʱ���ص�����*/
void my_button_scan(void)
{
	unsigned char i;
	unsigned int iCnt_old;
	
	for(i=0;i < g_tButtonDevcieManager.num;i++){
		iCnt_old = g_tButtonDevcieManager.button_list[i]->iCnt;
		
		 /* ��ⰴ���ĵ�ƽ״̬Ϊ����״��*/
		if(GPIO_ReadInputDataBit(GPIOA,g_tButtonDevcieManager.button_list[i]->iPin) == g_tButtonDevcieManager.button_list[i]->iPressLogicLevel){
			/* ����ɨ��ļ���ֵ��һ */
			g_tButtonDevcieManager.button_list[i]->iCnt ++;
			
			 /* �������µ�ʱ��ﵽ���������¼��������Ђ�*/
			if(g_tButtonDevcieManager.button_list[i]->iCnt == MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS){ //����ֻ��ִ��һ�K
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_CLICK_DOWN;
                MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
			/* �������µ�ʱ��ﵽ������ʼ�¼��������Ђ�*/
			else if(g_tButtonDevcieManager.button_list[i]->iCnt == MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_HOLD;
                MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
			/* �������µ�ʱ��ﵽ�������ڻص��¼��������Ђ�*/
			else if(g_tButtonDevcieManager.button_list[i]->iCnt > MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_HOLD_CYC;
				if(g_tButtonDevcieManager.button_list[i]->iHoldCycPeriod && g_tButtonDevcieManager.button_list[i]->iCnt % (g_tButtonDevcieManager.button_list[i]->iHoldCycPeriod / MY_BUTTON_SCAN_SPACE_MS)){
					MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));
				}			
			}
		}
		 /* ��ⰴ���ĵ�ƽ״̬Ϊ̧��״��*/
		else{
			/* ��������ļ�����*/
			g_tButtonDevcieManager.button_list[i]->iCnt = 0;
			/* �������µ�ʱ��ﵽ���������¼��������Ђ�*/
			if(iCnt_old >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS && iCnt_old < MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS ){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_CLICK_UP;
				MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
			 /* �������µ�ʱ��ﵽ���������¼��������Ђ�*/
			else if(MY_BUTTON_SCAN_SPACE_MS && iCnt_old >= MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_HOLD_UP;
				MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
		}	
	}
}

/*������������ʼ��*/
void __ButtonDevcieManagerInit(void)
{
	g_tButtonDevcieManager = *ConstrcutorButtonManage();
    
    /* ��ʼ������ɨ���õ��Ķ�ʱ�� */
    g_tButtonDevcieManager.ButtonTimerInit();
    
    /* �趨��ʱ���жϺ��� */
    SetTimerProcessFunction(my_button_scan);
}

//����������ô�����������rtt��ô������
static void GPIOButtonInit(int button_pin)
{
	KAL_GPIOButtonInit(button_pin);
}

/**********************************************************************
 * �������ƣ� ButtonRegister
 * ���������� ����ע�ᵽ����������
 * ��������� pButton:button deviceָ��
 *            name:��������,KEY1,KEY2,KEY3
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����		    �汾��			�޸���		   �޸�����
 * -----------------------------------------------
 * 
 ***********************************************************************/
 //����register������init����.
void ButtonRegister(struct Button *pButton,char *name)
{
	 /* ��ʼ��ButtonDevice��Ӧ�� pin ģʽ */
	if(pButton->iPressLogicLevel == 0){
		//�����ʼ���������Էŵ�struct Button,�����ͺ�struct LedDevice������
		//pButton->init(pButton->iPin);
		GPIOButtonInit(pButton->iPin);		
	}											

	/* ��ʼ��ButtonDevice�ṹ�� */
	pButton->name = name;
	pButton->iCnt = 0;
	pButton->eEvent = BUTTON_EVENT_NONE;
	
	/* ���ButtonDevice��������*/
	g_tButtonDevcieManager.button_list[g_tButtonDevcieManager.num++] = pButton;
}

/*���һ������*/
PButton __GetButton(char *name)
{
	PButton pbutton;
	
	for(int i=0;i < g_tButtonDevcieManager.num;i++){
		pbutton = g_tButtonDevcieManager.button_list[i];
		if(strcmp(pbutton->name,name) == 0){
			return pbutton;
		}
	}
    
	return NULL;
}





