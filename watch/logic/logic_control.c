#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <rtthread.h>

#include "drive_delay.h"
#include "input_system.h"
#include "input_buffer.h"
#include "logic_control.h"
#include "option.h"
#include "option_manager.h"
#include "player_system.h"
#include "message_manager.h"
#include "game_system.h"
#include "message_manager.h"
#include "convert.h"
#include "show_text.h"

static int player_volume = 50;
int ElementFlag = 0;

int iSubscribePlayerStatus;     //��������������Ϣ
struct menu_status tSubscribeMenuStatus;    //�˵���������Ϣ
static struct NormalTime tSubscribeRTCData;

/* Element ������*/
static void PlayerFirstKeyShortPressFun(void)
{	
	//printf("������״̬��:��һ�������̰�.\r\n");
	DefaultPlayerControl(PLAYER_CMD_LAST,&player_volume);
}

static void PlayerSecondKeyShortPressFun(void)
{	
	//printf("������״̬��:�ڶ��������̰�.\r\n");
	//����״̬ȷ������ͣ���Ż��ǿ�ʼ����
    if(iSubscribePlayerStatus == PLAYER_RUNNING){
        printf("��ͣ����.\r\n");
        DefaultPlayerControl(PLAYER_CMD_STOP,&player_volume);
    }
    else if(iSubscribePlayerStatus == PLAYER_STOP){
        printf("����.\r\n");
        DefaultPlayerControl(PLAYER_CMD_PLAY,&player_volume);
    }
	
}

static void PlayerThirdKeyShortPressFun(void)
{	
    //printf("������״̬��:�����������̰�.\r\n");
	DefaultPlayerControl(PLAYER_CMD_NEXT,&player_volume);
}

static void PlayerFirstKeyLongPressFun(void)
{	
	//printf("������״̬��:��һ����������.\r\n");
	player_volume--;
	if(player_volume < 0){
		player_volume = 0;
	}
	DefaultPlayerControl(PLAYER_CMD_SET_VOL,&player_volume);
}

extern rt_event_t g_tBackEvent;
static void PlayerSecondKeyLongPressFun(void)
{	
	//printf("������״̬��:�ڶ�����������.\r\n");
    //printf("��ʱ�൱�ڲ˵�״̬��:����.\r\n");
    
    DefaultPlayerControl(PLAYER_CMD_STOP,&player_volume);
    OptionControl(OPTION_CMD_BACK);

    ElementFlag = 0;
	/* ����һ���¼�,���Ѳ˵��߳� */
	rt_event_send(g_tBackEvent,GAME_EVENT); 

}

static void PlayerThirdKeyLongPressFun(void)
{	
	//printf("������״̬��:��������������.\r\n");
	
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

/* Element ��Ϸ */
static void GameFirstKeyShortPressFun(void)
{	
	//printf("��Ϸ״̬��:��һ�������̰�.\r\n");
    //printf("��ʱ�൱�ڲ˵�״̬��:����.\r\n");

    ElementFlag = 0;
 	OptionControl(OPTION_CMD_BACK);
	
	/* ����һ���¼�,���Ѳ˵��߳� */
	rt_event_send(g_tBackEvent,PLAYER_EVENT);
}

static void GameSecondKeyShortPressFun(void)
{	
	//printf("��Ϸ״̬��:�ڶ��������̰�.\r\n");
	DefaultGameControl(GAME_CMD_RESTART);	
}

static void GameThirdKeyShortPressFun(void)
{	
	//printf("��Ϸ״̬��:�����������̰�.\r\n");
	DefaultGameControl(GAME_CMD_ROLE_CONTROL);
}

static void GameFirstKeyLongPressFun(void)
{	
	//printf("��Ϸ״̬��:��һ����������.\r\n");
}

static void GameSecondKeyLongPressFun(void)
{	
	//printf("��Ϸ״̬��:�ڶ�����������.\r\n");
}

static void GameThirdKeyLongPressFun(void)
{	
	//printf("��Ϸ״̬��:��������������.\r\n");
}


Element g_tGameElement = { 
	GameFirstKeyShortPressFun,
	GameSecondKeyShortPressFun,
	GameThirdKeyShortPressFun,
  	GameFirstKeyLongPressFun, 
    GameSecondKeyLongPressFun,
    GameThirdKeyLongPressFun
};

/* Element �˵� */
static void MenuFirstKeyShortPressFun(void)
{	
	//printf("�˵�״̬��:��һ�������̰�.\r\n");
	OptionControl(OPTION_CMD_BACK);
}

static void MenuSecondKeyShortPressFun(void)
{	
	//printf("�˵�״̬��:�ڶ��������̰�.\r\n");
	OptionControl(OPTION_CMD_NEXT);
}

static void MenuThirdKeyShortPressFun(void)
{	
	//printf("�˵�״̬��:�����������̰�.\r\n");
	OptionControl(OPTION_CMD_ENTER);
}

static void MenuFirstKeyLongPressFun(void)
{	
	//printf("�˵�״̬��:��һ����������.\r\n");
	OptionControl(OPTION_CMD_BACK);
}

static void MenuSecondKeyLongPressFun(void)
{	
	//printf("�˵�״̬��:�ڶ�����������.\r\n");
	OptionControl(OPTION_CMD_NEXT);
}

static void MenuThirdKeyLongPressFun(void)
{	
	//printf("�˵�״̬��:��������������.\r\n");
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
	//��Ҫ�õ�Element������
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

//������Ϣ: ��������Ϣ
void HandlerPlayerMsg(void *pMsgData)
{   
	//printf("�����߲�����״̬��Ϣ,�����д���.\r\n");
    memcpy(&iSubscribePlayerStatus,pMsgData,4);
//    if(iSubscribePlayerStatus == PLAYER_RUNNING){
//        printf("��ǰ״̬Ϊ����״̬.\r\n");
//    }else{
//        printf("��ǰ״̬Ϊ��ͣ״̬.\r\n");
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
//������Ϣ: �˵���Ϣ
void HandlerMenuMsg(void *pMsgData)
{   
//	printf("Logic:�����߲˵�״̬��Ϣ,�����д���.\r\n");
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
//������Ϣ: RTC��Ϣ
void HandlerRTCMsg(void *pMsgData)
{   
    //printf("Logic:������RTC״̬��Ϣ,�����д���.\r\n");
    memcpy(&tSubscribeRTCData,pMsgData,28);
        
   if(rtc_enable){     
        ShowTextInDisplayDev(g_ptOledDev,40,16," 		");
        
        /* ʱ�� */
        if(tSubscribeRTCData.hour < 10){
            ShowNumberInDisplayDev(g_ptOledDev,40,16,0);
            ShowNumberInDisplayDev(g_ptOledDev,48,16,tSubscribeRTCData.hour);
        }else{
            ShowNumberInDisplayDev(g_ptOledDev,40,0,tSubscribeRTCData.hour);
        }
        ShowTextInDisplayDev(g_ptOledDev,57,16,":");
        
        /* ���� */
        if(tSubscribeRTCData.minutes < 10){
            ShowNumberInDisplayDev(g_ptOledDev,66,16,0);
            ShowNumberInDisplayDev(g_ptOledDev,74,16,tSubscribeRTCData.minutes);
        }else{
            ShowNumberInDisplayDev(g_ptOledDev,66,16,tSubscribeRTCData.minutes);
        }
          
        /* ���� */
        ShowTextInDisplayDev(g_ptOledDev,83,16,":");
        if(tSubscribeRTCData.second < 10){
            ShowNumberInDisplayDev(g_ptOledDev,93,16,0);
            ShowNumberInDisplayDev(g_ptOledDev,101,16,tSubscribeRTCData.second);
        }else{
            ShowNumberInDisplayDev(g_ptOledDev,93,16,tSubscribeRTCData.second);
        }
   }
}

struct Subcriber g_tSubcriberRTCMsg = {
	.Subscribe = SubscribeMsg,
	.HandleEvent = HandlerRTCMsg
};

void LogicThreadInit(void)
{
	printf("ҵ����ϵͳ��ʼ��.\r\n");

	//��ʼ��
	g_ptCurrentElement = &g_tMenuElement;
	
    //�ڻ����м��붩����
	g_tSubcriberPlayerMsg.Subscribe("player_topic",&g_tSubcriberPlayerMsg);
    g_tSubcriberMenuMsg.Subscribe("menu_topic",&g_tSubcriberMenuMsg);
    g_tSubcriberMenuMsg.Subscribe("rtc_topic",&g_tSubcriberRTCMsg);
}

/*�����߳̿��ƿ�*/
extern rt_thread_t g_ptLogicThread;
extern rt_thread_t g_ptMenuThread;
extern rt_thread_t g_ptPlayerThread;
extern rt_thread_t g_ptGameThread;

void LogicThreadEntry(void *arg)
{
	InputEvent event;
    static int visit_count = 0;
    
	printf("Logic Thread����.\r\n");  
	while(1){  
        if(ElementFlag == 0){
            g_ptCurrentElement = &g_tMenuElement;
        }
        else if(ElementFlag == 1){
            g_ptCurrentElement = &g_tPlayerElement;
        }
        else if(ElementFlag == 2)
        {
            g_ptCurrentElement = &g_tGameElement;
        }
        if(GetInputBuffer(&event) == 0){	
            //����һ�̰�
            if(event.iKey == 1 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                //�����ߵ��ڰ���һ�̰�
                g_ptCurrentVisitor = &g_tVisitorKEY1ShortPress;
                visit_count++;
            }   
            else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                //�����ߵ��ڰ������̰�
                g_ptCurrentVisitor = &g_tVisitorKEY2ShortPress;
                visit_count++;
            } 
            else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                //�����ߵ��ڰ������̰�
                g_ptCurrentVisitor = &g_tVisitorKEY3ShortPress;
                visit_count++;
            }
            else if(event.iKey == 1 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                //�����ߵ��ڰ���һ����
                g_ptCurrentVisitor = &g_tVisitorKEY1LongPress;
                visit_count++;
            }
            else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                //�����ߵ��ڰ���������
                g_ptCurrentVisitor = &g_tVisitorKEY2LongPress;
                visit_count++;
            }
            else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                //�����ߵ��ڰ���������
                g_ptCurrentVisitor = &g_tVisitorKEY3LongPress;
                visit_count++;
            }
        }
		/* �з����� */
		if(visit_count > 0){
            //printf("----�з�����----.\r\n");
            g_ptCurrentVisitor->visit(g_ptCurrentElement);
        
            visit_count--;
		}
		else{
			//printf("----û�з�����----.\r\n");
		}
	}
}

