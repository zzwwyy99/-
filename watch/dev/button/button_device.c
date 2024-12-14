#include <stdlib.h>
#include <string.h>

#include <kal_button.h>
#include <kal_timer.h>

#include "button_device.h"
#include "drive_timer.h"

#define MY_BUTTON_LIST_MAX	10

#define MY_BUTTON_CALL(func,argv) \
		   do{if((func) != 0) func argv;}while(0)
           
//按键管理器类
//这个定时器想来想去,最后还是按照rtt的加到这里确实是最好的,在输入设备或者放到按键结构体里面确实不太行
typedef struct ButtonDevcieManager{
	unsigned int num;               /* 已注册的按键数目 */
    void (*ButtonTimerInit)(void);  /* 按键扫描用到的定时器 */
	struct Button *button_list[MY_BUTTON_LIST_MAX];     /* 存储按键指针的链表*/
}ButtonDevcieManager;

static struct ButtonDevcieManager g_tButtonDevcieManager;

/* 定时器硬件初始化 */
static void ButtonTimerInit(void)
{
    KAL_TimerInit();
}

//单例模式
//析构函数:实现一个按键管理器
static ButtonDevcieManager *ConstrcutorButtonManage(void)
{
	static ButtonDevcieManager *ptManage;
	
	if(!ptManage){
		/* 关锁 */
		if(!ptManage){
			ptManage = (ButtonDevcieManager *)malloc(sizeof(struct ButtonDevcieManager));
			ptManage->num = 0;
            ptManage->ButtonTimerInit = ButtonTimerInit;
		}
		/* 开锁 */
	}
	return ptManage;
}


/* 定时器回调函数*/
void my_button_scan(void)
{
	unsigned char i;
	unsigned int iCnt_old;
	
	for(i=0;i < g_tButtonDevcieManager.num;i++){
		iCnt_old = g_tButtonDevcieManager.button_list[i]->iCnt;
		
		 /* 检测按键的电平状态为按下状怿*/
		if(GPIO_ReadInputDataBit(GPIOA,g_tButtonDevcieManager.button_list[i]->iPin) == g_tButtonDevcieManager.button_list[i]->iPressLogicLevel){
			/* 按键扫描的计数值加一 */
			g_tButtonDevcieManager.button_list[i]->iCnt ++;
			
			 /* 连续按下的时间达到单击按下事件触发的阈倿*/
			if(g_tButtonDevcieManager.button_list[i]->iCnt == MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS){ //单击只会执行一欿
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_CLICK_DOWN;
                MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
			/* 连续按下的时间达到长按开始事件触发的阈倿*/
			else if(g_tButtonDevcieManager.button_list[i]->iCnt == MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_HOLD;
                MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
			/* 连续按下的时间达到长按周期回调事件触发的阈倿*/
			else if(g_tButtonDevcieManager.button_list[i]->iCnt > MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_HOLD_CYC;
				if(g_tButtonDevcieManager.button_list[i]->iHoldCycPeriod && g_tButtonDevcieManager.button_list[i]->iCnt % (g_tButtonDevcieManager.button_list[i]->iHoldCycPeriod / MY_BUTTON_SCAN_SPACE_MS)){
					MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));
				}			
			}
		}
		 /* 检测按键的电平状态为抬起状怿*/
		else{
			/* 清除按键的计数倿*/
			g_tButtonDevcieManager.button_list[i]->iCnt = 0;
			/* 连续按下的时间达到单击结束事件触发的阈倿*/
			if(iCnt_old >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS && iCnt_old < MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS ){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_CLICK_UP;
				MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
			 /* 连续按下的时间达到长按结束事件触发的阈倿*/
			else if(MY_BUTTON_SCAN_SPACE_MS && iCnt_old >= MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){
				g_tButtonDevcieManager.button_list[i]->eEvent = BUTTON_EVENT_HOLD_UP;
				MY_BUTTON_CALL(g_tButtonDevcieManager.button_list[i]->cb,(g_tButtonDevcieManager.button_list[i]));		
			}
		}	
	}
}

/*按键管理器初始化*/
void __ButtonDevcieManagerInit(void)
{
	g_tButtonDevcieManager = *ConstrcutorButtonManage();
    
    /* 初始化按键扫描用到的定时器 */
    g_tButtonDevcieManager.ButtonTimerInit();
    
    /* 设定定时器中断函数 */
    SetTimerProcessFunction(my_button_scan);
}

//如果是裸机怎么样配置如果是rtt怎么样配置
static void GPIOButtonInit(int button_pin)
{
	KAL_GPIOButtonInit(button_pin);
}

/**********************************************************************
 * 函数名称： ButtonRegister
 * 功能描述： 按键注册到按键管理器
 * 输入参数： pButton:button device指针
 *            name:按键名称,KEY1,KEY2,KEY3
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期		    版本号			修改人		   修改内容
 * -----------------------------------------------
 * 
 ***********************************************************************/
 //整个register函数像init函数.
void ButtonRegister(struct Button *pButton,char *name)
{
	 /* 初始化ButtonDevice对应的 pin 模式 */
	if(pButton->iPressLogicLevel == 0){
		//这个初始化函数可以放到struct Button,这样就和struct LedDevice更相似
		//pButton->init(pButton->iPin);
		GPIOButtonInit(pButton->iPin);		
	}											

	/* 初始化ButtonDevice结构体 */
	pButton->name = name;
	pButton->iCnt = 0;
	pButton->eEvent = BUTTON_EVENT_NONE;
	
	/* 添加ButtonDevice到管理器*/
	g_tButtonDevcieManager.button_list[g_tButtonDevcieManager.num++] = pButton;
}

/*获得一个按键*/
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





