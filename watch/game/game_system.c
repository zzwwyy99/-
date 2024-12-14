#include "stm32f10x.h"
#include <stdio.h>
#include <rtthread.h>

#include "drive_delay.h"
#include "show_text.h"
#include "graph_data.h"
#include "display_system.h"
#include "role_manager.h"
#include "role_system.h"
#include "game.h"
#include "game_system.h"

extern PDisplayDevice g_ptOledDev;

/* 位图 */
GraphBitMap g_tGraphRESTART = {
	.iWidth = 24,
	.iHeight = 24,
	.pbuffer = (unsigned char *)RESTART
};

/* 位图 */
GraphBitMap g_tGraphSTART = {
	.iWidth = 128,
	.iHeight = 64,
	.pbuffer = (unsigned char *)START
};

extern rt_sem_t g_tGameSem;
int GameRunning(struct Game *ptGame)
{
	if(ptGame->eStatus != GAME_RUNNING){
		ptGame->eStatus = GAME_RUNNING;

		printf("游戏进入运行状态.\r\n");
		ClearInDisplayDev(g_ptOledDev,0,64,128,64);
        
        /* 恢复初始状态*/
        ptGame->ptIterator->traversal_recover_item(ptGame->ptIterator);	
        
        /* 发送信号量 */
   		rt_sem_release(g_tGameSem);
	}
 
	return 0;
}	

int GameStop(struct Game *ptGame)
{
	if(ptGame->eStatus != GAME_FAIlED){
		ptGame->eStatus = GAME_FAIlED;
	}
	
	/* 改变Game的其他参数 */
	if(ptGame->iCurrentScore > ptGame->iHighestScore){
		ptGame->iHighestScore = ptGame->iCurrentScore;
	}	
	ptGame->iCurrentScore = 0;

	/* 显示失败画面 */
	ShowGraphInDisplayDev(g_ptOledDev,50,40,&g_tGraphRESTART);
	ShowTextInDisplayDev(g_ptOledDev,10,40,"GAME");
	ShowTextInDisplayDev(g_ptOledDev,86,40,"OVER");
			
    //ptGame->ptIterator->traversal_recover_item(ptGame->ptIterator);	
   	
	return 0;
}

int GameControl(Game *ptGame,int iCmd)
{	
	switch(iCmd){
		case GAME_CMD_ROLE_CONTROL:
			DefaultDinosaurJUMP();		//1.控制恐龙起跳
			break;
		case GAME_CMD_RESTART:
			GameRunning(ptGame);		//2.恢复数据						
			break;
		case GAME_CMD_BACK:
			//
			break;
	}
	return 0;
}

static PGame g_ptGame;
int DefaultGameControl(int iCmd)
{	
	switch(iCmd){
		case GAME_CMD_ROLE_CONTROL:
            if(g_ptGame->eStatus == GAME_RUNNING){
                DefaultDinosaurJUMP();		//1.控制恐龙起跳
            }
			break;
		case GAME_CMD_RESTART:          
			GameRunning(g_ptGame);		//2.恢复数据						
			break;
		case GAME_CMD_BACK:
			//
			break;
	}
	return 0;
}

void GameShow(struct Game *ptGame)
{
	/*显示当前分以及最高分*/
	ShowTextInDisplayDev(g_ptOledDev,35,16,"HI:"); 
	ShowNumberInDisplayDev(g_ptOledDev,58,16,ptGame->iHighestScore); 
	ShowNumberInDisplayDev(g_ptOledDev,92,16,ptGame->iCurrentScore); 
}

PGame GetGame(void)
{
	return __GetGame();
}

extern PDisplayDevice g_ptOledDev;

void GameThreadInit(void)
{
    //添加ROLE到迭代器，以及初始化默认的ROLE
    AddRole();
	
	g_ptGame = GetGame();
}

void GameThreadEntry(void *arg)
{
    /* 显示启动画面 */
	ShowGraphInDisplayDev(g_ptOledDev,0,64,&g_tGraphSTART);
	
	//清除屏幕
    for(int iy=0;iy<g_ptOledDev->iYres;iy++){
        for(int ix = 0;ix < g_ptOledDev->iXres;ix++){
            g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,0);
        }          
    }
	
	printf("Game Thread启动.\r\n");
    
    /* 等待播放的信号量 */
    rt_sem_take(g_tGameSem, RT_WAITING_FOREVER);
    
    rt_thread_delay(300);
    while(1){
		//RUNNING状态
		if(g_ptGame->eStatus == GAME_RUNNING){     
            //增加分数
			g_ptGame->iCurrentScore++;
		
			/* 遍历ROLE */
			g_ptGame->ptIterator->traversal_excute_item(g_ptGame->ptIterator);
			
			//增加LEVEL
			//g_ptGame->iLevel = 35 - (gamedino->game_score)/200;
//			if(gamedino->game_score > 4000){
//				gamedino->game_level = 15;
//			}

			/* 显示分数等 */
			GameShow(g_ptGame);
           
            //关闭中断
            //g_ptOledDev->flush(g_ptOledDev);       //被打断OLED画面会移位
            //delay(200);
            rt_thread_delay(10);
		}
		//STOP状态
		else{
            //自动刷新
            //TIMER_FLUSH(ENABLE);

			/* 等待播放的信号量 */
           	rt_sem_take(g_tGameSem, RT_WAITING_FOREVER);
		}       
	}
}


