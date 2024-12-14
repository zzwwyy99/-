#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stm32f10x.h>
#include <rthw.h>

#include "graph_data.h"
#include "show_graph.h"
#include "drive_delay.h"
#include "show_text.h"
#include "display_system.h"
#include "node_manager.h"
#include "option_manager.h"
#include "dht11_device.h"
#include "led_system.h"

extern PDisplayDevice g_ptOledDev;
int enter_node_count = 0;  //不等于0,则刷新一次屏幕.进入子结点的次数.
int enter_status_count = 0;

/*************************
 * 0: HOME
 **************************/
/* 位图 */
GraphBitMap g_tMenuGraphSIGNAL = {
	.iWidth = 20,
	.iHeight = 24,
	.pbuffer = (unsigned char *)SIGNAL
};

/* 位图 */
GraphBitMap g_tMenuGraphBLUETOOTH = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)BLUETOOTH
};

/* 位图 */
GraphBitMap g_tMenuGraphBATTERY = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)BATTERY
};

/* 位图 */
GraphBitMap g_tMenuGraphLEFT = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)LEFT
};

GraphBitMap g_tMenuGraphRIGHT = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)RIGHT
};
	
/* 位图 */
GraphBitMap g_tMenuGraphHOME = {
	.iWidth = 48,
	.iHeight = 48,
	.pbuffer = (unsigned char *)HOME
};

GraphBitMap g_tMenuGraphYES = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)YES
};

GraphBitMap g_tMenuGraphBACK = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)BACK
};

void HomeChooseFUN(struct Option *ptOption)
{	
	ShowGraphInDisplayDev(g_ptOledDev,0,24,&g_tMenuGraphSIGNAL);
	ShowGraphInDisplayDev(g_ptOledDev,20,16,&g_tMenuGraphBLUETOOTH);
	ShowGraphInDisplayDev(g_ptOledDev,112,16,&g_tMenuGraphBATTERY);
	ShowGraphInDisplayDev(g_ptOledDev,12,48,&g_tMenuGraphLEFT);
	ShowGraphInDisplayDev(g_ptOledDev,100,48,&g_tMenuGraphRIGHT);
	ShowGraphInDisplayDev(g_ptOledDev,40,64,&g_tMenuGraphHOME);
	ShowGraphInDisplayDev(g_ptOledDev,4,64,&g_tMenuGraphYES);
	ShowGraphInDisplayDev(g_ptOledDev,108,64,&g_tMenuGraphBACK);
}

void HomeEnterFUN(struct Option *ptOption)
{	
	/* 切换为子结点 */
	//有子结点,这里永远不会到达
	printf("永远不会到达.否则错误.\r\n");
}

struct Option g_tOptionHOME = {
	.node.parent = -1,
	.node.first_child = 1,	
	.node.right_sibling = -1,

	.status = OPTION_CHOOSE,
	.choose = HomeChooseFUN,
	.enter = HomeEnterFUN
};


/*************************
 * 1: TEMP
 **************************/
GraphBitMap g_tMenuGraphTEMP = {
	.iWidth = 48,
	.iHeight = 48,
	.pbuffer = (unsigned char *)TEMP
};
	
void TempatureChooseFUN(struct Option *ptOption)
{	
    //表示从子结点出来
	if(enter_node_count != 0){
		ClearInDisplayDev(g_ptOledDev,0,64,128,64);
		enter_node_count--;
        
        ShowGraphInDisplayDev(g_ptOledDev,0,24,&g_tMenuGraphSIGNAL);
        ShowGraphInDisplayDev(g_ptOledDev,20,16,&g_tMenuGraphBLUETOOTH);
        ShowGraphInDisplayDev(g_ptOledDev,112,16,&g_tMenuGraphBATTERY);
        ShowGraphInDisplayDev(g_ptOledDev,12,48,&g_tMenuGraphLEFT);
        ShowGraphInDisplayDev(g_ptOledDev,100,48,&g_tMenuGraphRIGHT);
        ShowGraphInDisplayDev(g_ptOledDev,4,64,&g_tMenuGraphYES);
        ShowGraphInDisplayDev(g_ptOledDev,108,64,&g_tMenuGraphBACK);
	}
    
	ClearInDisplayDev(g_ptOledDev,40,64,48,48);
	ShowGraphInDisplayDev(g_ptOledDev,40,64,&g_tMenuGraphTEMP);
}

void TempatureEnterFUN(struct Option *ptOption)
{	   
    rt_base_t level;
    char dht11_data[5];
    PLedDevice ptLed = GetLedDevice("RED");;  
  
    //表示进入子结点
	if(enter_node_count == 0){
        printf("开始温湿度检测.\r\n");
		ClearInDisplayDev(g_ptOledDev,0,64,128,64);
		enter_node_count++;
	}
    
    ptLed->control(ptLed,LED_TROGGLE);

    /* 关中断 */
    //level = rt_hw_interrupt_disable();

    if(DHT11ReceiveData(dht11_data)){
//      printf("Temperature:%d.%d   ",dht11_data[2],dht11_data[3]);
//      printf("Humidity:%d.%d\r\n",dht11_data[0],dht11_data[1]);
        
        ShowTextInDisplayDev(g_ptOledDev,0,16,"Temperature:");	
        ShowNumberInDisplayDev(g_ptOledDev,95,16,dht11_data[2]);	
        ShowTextInDisplayDev(g_ptOledDev,110,16,".");	
        ShowNumberInDisplayDev(g_ptOledDev,115,16,dht11_data[3]);
        
        ShowTextInDisplayDev(g_ptOledDev,0,32,"Humidity:");	
        ShowNumberInDisplayDev(g_ptOledDev,70,32,dht11_data[0]);	
        ShowTextInDisplayDev(g_ptOledDev,86,32,".");	
        ShowNumberInDisplayDev(g_ptOledDev,90,32,dht11_data[1]);
        
    }

    Delay_ms(1000);
    /* 开中断 */
    //rt_hw_interrupt_enable(level);
    
}

struct Option g_tOptionTEMPATURE = {
	.node.parent = 0,
	.node.first_child = -1,
	.node.right_sibling = 2,

	.status = OPTION_CHOOSE,
	.choose = TempatureChooseFUN,
	.enter = TempatureEnterFUN,
};

/*************************
 * 2: GAME
 **************************/
 GraphBitMap g_tMenuGraphGAME = {
	.iWidth = 48,
	.iHeight = 48,
	.pbuffer = (unsigned char *)GAME
};

void GameChooseFUN(struct Option *ptOption)
{	
	//表示从子结点出来
	if(enter_node_count != 0){
		ClearInDisplayDev(g_ptOledDev,0,64,128,64);
		enter_node_count--;
        
        ShowGraphInDisplayDev(g_ptOledDev,0,24,&g_tMenuGraphSIGNAL);
        ShowGraphInDisplayDev(g_ptOledDev,20,16,&g_tMenuGraphBLUETOOTH);
        ShowGraphInDisplayDev(g_ptOledDev,112,16,&g_tMenuGraphBATTERY);
        ShowGraphInDisplayDev(g_ptOledDev,12,48,&g_tMenuGraphLEFT);
        ShowGraphInDisplayDev(g_ptOledDev,100,48,&g_tMenuGraphRIGHT);
        ShowGraphInDisplayDev(g_ptOledDev,4,64,&g_tMenuGraphYES);
        ShowGraphInDisplayDev(g_ptOledDev,108,64,&g_tMenuGraphBACK);
	}
    
	ClearInDisplayDev(g_ptOledDev,40,64,48,48);	
    ShowGraphInDisplayDev(g_ptOledDev,40,64,&g_tMenuGraphGAME);
}

void GameEnterFUN(struct Option *ptOption)
{	
	printf("永远不会到达.否则错误.\r\n");
}

struct Option g_tOptionGAME = {
	.node.parent = 0,
	.node.first_child = 6,
	.node.right_sibling = 3,

	.status = OPTION_CHOOSE,
	.choose = GameChooseFUN,
	.enter = GameEnterFUN,
};

/*************************
 * 3: PLAYER
 **************************/
 GraphBitMap g_tMenuGraphPLAYER = {
	.iWidth = 32,
	.iHeight = 32,
	.pbuffer = (unsigned char *)PLAYER
};

void PlayerChooseFUN(struct Option *ptOption)
{	
    //表示从子结点出来
	if(enter_node_count != 0){
        ClearInDisplayDev(g_ptOledDev,0,64,128,64);
		enter_node_count--;
        
        ShowGraphInDisplayDev(g_ptOledDev,0,24,&g_tMenuGraphSIGNAL);
        ShowGraphInDisplayDev(g_ptOledDev,20,16,&g_tMenuGraphBLUETOOTH);
        ShowGraphInDisplayDev(g_ptOledDev,112,16,&g_tMenuGraphBATTERY);
        ShowGraphInDisplayDev(g_ptOledDev,12,48,&g_tMenuGraphLEFT);
        ShowGraphInDisplayDev(g_ptOledDev,100,48,&g_tMenuGraphRIGHT);
        ShowGraphInDisplayDev(g_ptOledDev,4,64,&g_tMenuGraphYES);
        ShowGraphInDisplayDev(g_ptOledDev,108,64,&g_tMenuGraphBACK);
	}
    
	ClearInDisplayDev(g_ptOledDev,40,64,48,48);
	ShowGraphInDisplayDev(g_ptOledDev,48,56,&g_tMenuGraphPLAYER);
	
}

extern struct rt_thread g_tPlayerThread;
extern struct rt_thread *g_ptScheduleThread;
void PlayerEnterFUN(struct Option *ptOption)
{	
    //进入子结点
	if(enter_node_count == 0){
		ClearInDisplayDev(g_ptOledDev,0,64,128,64);
		enter_node_count++;
        
        printf("开始播放歌曲.\r\n");
        ShowTextInDisplayDev(g_ptOledDev,0,16,"Player...");
        
        //线程切换
        //记录要切换去的线程
        g_ptScheduleThread = &g_tPlayerThread;
        //rt_schedule(&g_tPlayerThread);
    }	
}

struct Option g_tOptionPLAYER = {
	.node.parent = 0,
	.node.first_child = -1,
	.node.right_sibling = 4,

	.status = OPTION_CHOOSE,
	.choose = PlayerChooseFUN,
	.enter = PlayerEnterFUN,
};

/*************************
 * 4: SETTING
 **************************/
 GraphBitMap g_tMenuGraphSETTING = {
	.iWidth = 48,
	.iHeight = 48,
	.pbuffer = (unsigned char *)SETTING
};

void SetChooseFUN(struct Option *ptOption)
{	
	ClearInDisplayDev(g_ptOledDev,40,64,48,48);
	ShowGraphInDisplayDev(g_ptOledDev,40,64,&g_tMenuGraphSETTING);
	
}

void SetEnterFUN(struct Option *ptOption)
{	
	static int inited = 0;
	if(inited == 0)
		printf("开始设置.\r\n");
	inited = 1;

	//printf("永远不会到达.否则错误.\r\n");
}

struct Option g_tOptionSETTING = {
	.node.parent = 0,
	.node.first_child = -1,
	.node.right_sibling = 5,

	.status = OPTION_CHOOSE,
	.choose = SetChooseFUN,
	.enter = SetEnterFUN,
};

/*************************
 * 5: INFORMATION
 **************************/
 GraphBitMap g_tMenuGraphINFORMATION = {
	.iWidth = 48,
	.iHeight = 48,
	.pbuffer = (unsigned char *)INFORMATION
};

void InfoChooseFUN(struct Option *ptOption)
{	
	ClearInDisplayDev(g_ptOledDev,40,64,48,48);
	ShowGraphInDisplayDev(g_ptOledDev,40,64,&g_tMenuGraphINFORMATION);
    
}

void InfoEnterFUN(struct Option *ptOption)
{	
	static int inited = 0;
	if(inited == 0)
		printf("信息.\r\n");
	inited = 1;

	//printf("永远不会到达.否则错误.\r\n");
}

struct Option g_tOptionINFORMATION = {
	.node.parent = 0,
	.node.first_child = -1,
	.node.right_sibling = 1,

	.status = OPTION_CHOOSE,
	.choose = InfoChooseFUN,
	.enter = InfoEnterFUN,
};

/*************************
 * 6: GAMEStart
 **************************/
void GAMEStartOptionChooseFUN(struct Option *ptOption)
{	
	//表示进入子结点
	if(enter_node_count == 0){
		ClearInDisplayDev(g_ptOledDev,0,64,128,64);
		enter_node_count++;
	}
     //从子结点出来
	if(enter_status_count != 0){
        ClearInDisplayDev(g_ptOledDev,0,64,128,64);	
        
        enter_status_count--;	
	}
    
    ShowTextInDisplayDev(g_ptOledDev,10,16,"START");
    ShowTextInDisplayDev(g_ptOledDev,10,32,"SET");
    ShowTextInDisplayDev(g_ptOledDev,10,48,"INFO");
    ShowTextInDisplayDev(g_ptOledDev,55,48,"  ");	
	ShowTextInDisplayDev(g_ptOledDev,55,16,"<-");		 	
}

extern struct rt_thread g_tGameThread;
void GAMEStartOptionEnterFUN(struct Option *ptOption)
{  
    //表示进入子结点
	if(enter_status_count == 0){
        enter_status_count++;
        
        //记录要切换去的线程
		g_ptScheduleThread = &g_tGameThread;	
	}
}

struct Option g_tOptionGAMEStart = {
	.node.parent = 2,
	.node.first_child = -1,
	.node.right_sibling = 7,

	.status = OPTION_CHOOSE,
	.choose = GAMEStartOptionChooseFUN,
	.enter = GAMEStartOptionEnterFUN,
};

/*************************
 * 7: GAMESet
 **************************/
void GAMESetOptionChooseFUN(struct Option *ptOption)
{	
	ShowTextInDisplayDev(g_ptOledDev,10,16,"START");
	ShowTextInDisplayDev(g_ptOledDev,10,32,"SET");
	ShowTextInDisplayDev(g_ptOledDev,10,48,"INFO");
	ShowTextInDisplayDev(g_ptOledDev,55,16,"  ");	
	ShowTextInDisplayDev(g_ptOledDev,55,32,"<-");	
}

void GAMESetOptionEnterFUN(struct Option *ptOption)
{	
	static int inited = 0;
	if(inited == 0)
		printf("游戏设置.\r\n");
	inited = 1;
}

struct Option g_tOptionGAMESet = {
	.node.parent = 2,
	.node.first_child = -1,
	.node.right_sibling = 8,

	.status = OPTION_CHOOSE,
	.choose = GAMESetOptionChooseFUN,
	.enter = GAMESetOptionEnterFUN,
};

/*************************
 * 8: GAMEInfo
 **************************/
void GAMEInfoOptionChooseFUN(struct Option *ptOption)
{			
	ShowTextInDisplayDev(g_ptOledDev,10,16,"START");
	ShowTextInDisplayDev(g_ptOledDev,10,32,"SET");
	ShowTextInDisplayDev(g_ptOledDev,10,48,"INFO");
	ShowTextInDisplayDev(g_ptOledDev,55,32,"  ");	
	ShowTextInDisplayDev(g_ptOledDev,55,48,"<-");	
}

void GAMEInfoOptionEnterFUN(struct Option *ptOption)
{	
	static int inited = 0;
	if(inited == 0)
		printf("游戏信息.\r\n");
	inited = 1;

	//printf("永远不会到达.否则错误.\r\n");
}

struct Option g_tOptionGAMEInfo = {
	.node.parent = 2,
	.node.first_child = -1,
	.node.right_sibling = 6,

	.status = OPTION_CHOOSE,
	.choose = GAMEInfoOptionChooseFUN,
	.enter = GAMEInfoOptionEnterFUN,
};
	
void AddOptionToManager(void)
{
	OptionRegister(&g_tOptionHOME,"home");
	OptionRegister(&g_tOptionTEMPATURE,"tempature");
	OptionRegister(&g_tOptionGAME,"game");
	OptionRegister(&g_tOptionPLAYER,"player");
	OptionRegister(&g_tOptionSETTING,"set");
	OptionRegister(&g_tOptionINFORMATION,"info");

	OptionRegister(&g_tOptionGAMEStart,"game_start");
	OptionRegister(&g_tOptionGAMESet,"game_set");
	OptionRegister(&g_tOptionGAMEInfo,"game_info");
}


