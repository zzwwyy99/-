#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <rtthread.h>

#include "drive_delay.h"
#include "input_system.h"
#include "input_buffer.h"
#include "logic_control.h"
#include "option_manager.h"
#include "player_system.h"
#include "message_manager.h"
#include "game_system.h"
#include "message_manager.h"
#include "convert.h"
#include "show_text.h"

static int player_volume = 50;

extern struct rt_thread g_tMenuThread;
extern struct rt_thread g_tPlayerThread;
extern struct rt_thread g_tGameThread;
extern struct rt_thread g_tLogicThread;
extern struct rt_thread *g_ptScheduleThread;

int iSubscribePlayerStatus;     //播放器发来的消息
struct menu_status tSubscribeMenuStatus;    //菜单发来的消息
static struct NormalTime tSubscribeRTCData;

/* Element 播放器*/
static void PlayerFirstKeyShortPressFun(void)
{	
	printf("播放器状态下:第一个按键短按.\r\n");
	DefaultPlayerControl(PLAYER_CMD_LAST,&player_volume);
}

static void PlayerSecondKeyShortPressFun(void)
{	
	printf("播放器状态下:第二个按键短按.\r\n");
	//根据状态确定是暂停播放还是开始播放
    if(iSubscribePlayerStatus == PLAYER_RUNNING){
        printf("暂停播放.\r\n");
        DefaultPlayerControl(PLAYER_CMD_STOP,&player_volume);
    }
    else if(iSubscribePlayerStatus == PLAYER_STOP){
        printf("播放.\r\n");
        DefaultPlayerControl(PLAYER_CMD_PLAY,&player_volume);
    }
	
}

static void PlayerThirdKeyShortPressFun(void)
{	
	printf("播放器状态下:第三个按键短按.\r\n");
	DefaultPlayerControl(PLAYER_CMD_NEXT,&player_volume);
}

static void PlayerFirstKeyLongPressFun(void)
{	
	printf("播放器状态下:第一个按键长按.\r\n");
	player_volume--;
	if(player_volume < 0){
		player_volume = 0;
	}
	DefaultPlayerControl(PLAYER_CMD_SET_VOL,&player_volume);
}

static void PlayerSecondKeyLongPressFun(void)
{	
	printf("播放器状态下:第二个按键长按.\r\n");
    printf("此时相当于菜单状态下:返回.\r\n");
    
    DefaultPlayerControl(PLAYER_CMD_STOP,&player_volume);
    OptionControl(OPTION_CMD_BACK);
    g_ptScheduleThread = &g_tMenuThread;
}

static void PlayerThirdKeyLongPressFun(void)
{	
	printf("播放器状态下:第三个按键长按.\r\n");
	
	player_volume++;
	if(player_volume > 100){
		player_volume = 100;
	}	
	DefaultPlayerControl(PLAYER_CMD_SET_VOL,&player_volume);
}

Element g_tPlayerElement = { 
	PlayerFirstKeyShortPressFun,
	PlayerSecondKeyShortPressFun,
	PlayerThirdKeyShortPressFun,
    PlayerFirstKeyLongPressFun, 
    PlayerSecondKeyLongPressFun,
    PlayerThirdKeyLongPressFun
};

/* Element 游戏 */
static void GameFirstKeyShortPressFun(void)
{	
	printf("游戏状态下:第一个按键短按.\r\n");
    printf("此时相当于菜单状态下:返回.\r\n");
	g_ptScheduleThread = &g_tMenuThread;
    OptionControl(OPTION_CMD_BACK);
}

static void GameSecondKeyShortPressFun(void)
{	
	printf("游戏状态下:第二个按键短按.\r\n");
	DefaultGameControl(GAME_CMD_RESTART);	
}

static void GameThirdKeyShortPressFun(void)
{	
	printf("游戏状态下:第三个按键短按.\r\n");
	DefaultGameControl(GAME_CMD_ROLE_CONTROL);
}

static void GameFirstKeyLongPressFun(void)
{	
	printf("游戏状态下:第一个按键长按.\r\n");
}

static void GameSecondKeyLongPressFun(void)
{	
	printf("游戏状态下:第二个按键长按.\r\n");
}

static void GameThirdKeyLongPressFun(void)
{	
	printf("游戏状态下:第三个按键长按.\r\n");
}


Element g_tGameElement = { 
	GameFirstKeyShortPressFun,
	GameSecondKeyShortPressFun,
	GameThirdKeyShortPressFun,
  	GameFirstKeyLongPressFun, 
    GameSecondKeyLongPressFun,
    GameThirdKeyLongPressFun
};

/* Element 菜单 */
static void MenuFirstKeyShortPressFun(void)
{	
	printf("菜单状态下:第一个按键短按.\r\n");
	OptionControl(OPTION_CMD_BACK);

	//调度到Menu线程
	//rt_schedule(&g_tMenuThread);
}

static void MenuSecondKeyShortPressFun(void)
{	
	printf("菜单状态下:第二个按键短按.\r\n");
	OptionControl(OPTION_CMD_NEXT);
}

static void MenuThirdKeyShortPressFun(void)
{	
	printf("菜单状态下:第三个按键短按.\r\n");
	OptionControl(OPTION_CMD_ENTER);
}

static void MenuFirstKeyLongPressFun(void)
{	
	printf("菜单状态下:第一个按键长按.\r\n");
	OptionControl(OPTION_CMD_BACK);
}

static void MenuSecondKeyLongPressFun(void)
{	
	printf("菜单状态下:第二个按键长按.\r\n");
	OptionControl(OPTION_CMD_NEXT);
}

static void MenuThirdKeyLongPressFun(void)
{	
	printf("菜单状态下:第三个按键长按.\r\n");
	OptionControl(OPTION_CMD_ENTER);
}

Element g_tMenuElement = { 
	MenuFirstKeyShortPressFun,
	MenuSecondKeyShortPressFun,
	MenuThirdKeyShortPressFun,
  	MenuFirstKeyLongPressFun, 
    MenuSecondKeyLongPressFun,
    MenuThirdKeyLongPressFun
};

/* Visitor  1 */
static void FKSPVisit(struct Element *ptElement)
{
	//需要用到Element的数据
    ptElement->FirstKeyShortPress();
}

struct Visitor g_tVisitorKEY1ShortPress = { 
	.visit = FKSPVisit 
};

/* Visitor  2 */
static void SKSPVisit(struct Element *ptElement)
{
    ptElement->SecondKeyShortPress();
}

struct Visitor g_tVisitorKEY2ShortPress = { 
	.visit = SKSPVisit 
};

/* Visitor  3 */
static void TKSPVisit(struct Element *ptElement)
{
    ptElement->ThirdKeyShortPress();
}

struct Visitor g_tVisitorKEY3ShortPress = { 
	.visit = TKSPVisit 
};
	

/* Visitor  4 */
static void FKLPVisit(struct Element *ptElement)
{
    ptElement->FirstKeyLongPress();
}

struct Visitor g_tVisitorKEY1LongPress = { 
	.visit = FKLPVisit 
};

/* Visitor  5 */
static void SKLPVisit(struct Element *ptElement)
{
    ptElement->SecondKeyLongPress();
}

struct Visitor g_tVisitorKEY2LongPress = { 
	.visit = SKLPVisit 
};

/* Visitor  6 */
static void TKLPVisit(struct Element *ptElement)
{
    ptElement->ThirdKeyLongPress();
}

struct Visitor g_tVisitorKEY3LongPress = { 
	.visit = TKLPVisit 
};


static struct Visitor *g_ptCurrentVisitor = 0;
static struct Element *g_ptCurrentElement = 0;

//接受消息: 播放器消息
void HandlerPlayerMsg(void *pMsgData)
{   
	printf("订阅者播放器状态信息,并进行处理.\r\n");
    memcpy(&iSubscribePlayerStatus,pMsgData,4);
//    if(iSubscribePlayerStatus == PLAYER_RUNNING){
//        printf("当前状态为播放状态.\r\n");
//    }else{
//        printf("当前状态为暂停状态.\r\n");
//    }
}

static void SubscribeMsg(char *pcTopicName, struct Subcriber* ptSubcriber)
{
	RegisterSubscriber(pcTopicName,ptSubcriber);
}

struct Subcriber g_tSubcriberPlayerMsg = {
	.Subscribe = SubscribeMsg,
	.HandleEvent = HandlerPlayerMsg
};

static int rtc_enable = 0;
//接受消息: 菜单消息
void HandlerMenuMsg(void *pMsgData)
{   
//	printf("Logic:订阅者菜单状态信息,并进行处理.\r\n");
    memcpy(&tSubscribeMenuStatus,pMsgData,8);
    
    if(tSubscribeMenuStatus.EnterNodeCount == 0 && tSubscribeMenuStatus.EnterStatusCount == 0){
        rtc_enable = 1;
    }else{
        rtc_enable = 0;
    }
}

struct Subcriber g_tSubcriberMenuMsg = {
	.Subscribe = SubscribeMsg,
	.HandleEvent = HandlerMenuMsg
};

extern PDisplayDevice g_ptOledDev;
//接受消息: RTC消息
void HandlerRTCMsg(void *pMsgData)
{   
    //printf("Logic:订阅者RTC状态信息,并进行处理.\r\n");
    memcpy(&tSubscribeRTCData,pMsgData,28);
        
   if(rtc_enable){     
        ShowTextInDisplayDev(g_ptOledDev,40,16," 		");
        
        /* 时钟 */
        if(tSubscribeRTCData.hour < 10){
            ShowNumberInDisplayDev(g_ptOledDev,40,16,0);
            ShowNumberInDisplayDev(g_ptOledDev,48,16,tSubscribeRTCData.hour);
        }else{
            ShowNumberInDisplayDev(g_ptOledDev,40,0,tSubscribeRTCData.hour);
        }
        ShowTextInDisplayDev(g_ptOledDev,57,16,":");
        
        /* 分钟 */
        if(tSubscribeRTCData.minutes < 10){
            ShowNumberInDisplayDev(g_ptOledDev,66,16,0);
            ShowNumberInDisplayDev(g_ptOledDev,74,16,tSubscribeRTCData.minutes);
        }else{
            ShowNumberInDisplayDev(g_ptOledDev,66,16,tSubscribeRTCData.minutes);
        }
          
        /* 秒钟 */
        ShowTextInDisplayDev(g_ptOledDev,83,16,":");
        if(tSubscribeRTCData.second < 10){
            ShowNumberInDisplayDev(g_ptOledDev,93,16,0);
            ShowNumberInDisplayDev(g_ptOledDev,101,16,tSubscribeRTCData.second);
        }else{
            ShowNumberInDisplayDev(g_ptOledDev,93,16,tSubscribeRTCData.second);
        }
   }
   //printf("%d:%d:%d:%d:%d:%d:%d.\r\n",tSubscribeRTCData.year,tSubscribeRTCData.month,tSubscribeRTCData.day,tSubscribeRTCData.hour,tSubscribeRTCData.minutes,tSubscribeRTCData.second,tSubscribeRTCData.weekday);
}

struct Subcriber g_tSubcriberRTCMsg = {
	.Subscribe = SubscribeMsg,
	.HandleEvent = HandlerRTCMsg
};

void LogicThreadInit(void)
{
	printf("Logic Thread Init.\r\n");

	//初始化
	g_ptCurrentElement = &g_tMenuElement;
	
    //在话题中加入订阅者
	g_tSubcriberPlayerMsg.Subscribe("player_topic",&g_tSubcriberPlayerMsg);
    g_tSubcriberMenuMsg.Subscribe("menu_topic",&g_tSubcriberMenuMsg);
    g_tSubcriberMenuMsg.Subscribe("rtc_topic",&g_tSubcriberRTCMsg);
}

//设想中是没按下一次按键,就调度到这里,从缓冲区读出一个输入,然后进行处理
//存在来不及处理的情况.比如加了串口打印输入信息后,就来不及处理.为啥呢?是超过了10ms?
void LogicThreadEntry(void *arg)
{
	InputEvent event;
    static int visit_count = 0;
    
	printf("Logic Thread.\r\n");    
	while(1){  
        if(g_ptScheduleThread == &g_tMenuThread){
            g_ptCurrentElement = &g_tMenuElement;
        }
        else if(g_ptScheduleThread == &g_tPlayerThread){
            g_ptCurrentElement = &g_tPlayerElement;
        }
        else if(g_ptScheduleThread == &g_tGameThread)
        {
            g_ptCurrentElement = &g_tGameElement;
        }
        else if(g_ptScheduleThread == &g_tLogicThread)
        {
            printf("不会到达这里,否则发生错误.\r\n");   //如果不把g_tLogicThread做成线程,可以不用g_ptScheduleThread
        }
        
        if(GetInputBuffer(&event) == 0){	
            //按键一短按
            if(event.iKey == 1 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                //访问者等于按键一短按
                g_ptCurrentVisitor = &g_tVisitorKEY1ShortPress;
                visit_count++;
            }   
            else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                //访问者等于按键二短按
                g_ptCurrentVisitor = &g_tVisitorKEY2ShortPress;
                visit_count++;
            } 
            else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                //访问者等于按键三短按
                g_ptCurrentVisitor = &g_tVisitorKEY3ShortPress;
                visit_count++;
            }
            else if(event.iKey == 1 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                //访问者等于按键一长按
                g_ptCurrentVisitor = &g_tVisitorKEY1LongPress;
                visit_count++;
            }
            else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                //访问者等于按键二长按
                g_ptCurrentVisitor = &g_tVisitorKEY2LongPress;
                visit_count++;
            }
            else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                //访问者等于按键三长按
                g_ptCurrentVisitor = &g_tVisitorKEY3LongPress;
                visit_count++;
            }
        }

		/* 有访问者 */
		if(visit_count > 0){
                printf("----有访问者----.\r\n");
                g_ptCurrentVisitor->visit(g_ptCurrentElement);
            
                visit_count--;
		}
		else{
			printf("----没有访问者----.\r\n");
			//rt_thread_suspend();
		}
		rt_schedule(g_ptScheduleThread);
	}
}

