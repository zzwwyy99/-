#include <stm32f10x.h>
#include <stdio.h>

#include "graph_data.h"
#include "game_system.h"
#include "drive_delay.h"
#include "show_graph.h"
#include "show_text.h"
#include "role_system.h"
#include "display_system.h"

#include "button_system.h"
#include "input_system.h"
#include "input_buffer.h"

extern PDisplayDevice g_ptOledDev;

/* 位图 */
extern GraphBitMap g_tGraphRESTART;
/* 位图 */
extern GraphBitMap g_tGraphSTART;

void game_test(void)
{
    InputEvent event;
	PGame ptGame;

	ptGame = GetGame();
    
    AddRole();
    
    printf("game test:\r\n");
    /* 显示启动画面 */
	ShowGraphInDisplayDev(g_ptOledDev,0,64,&g_tGraphSTART);
	delay(15000);
	//清除屏幕
	//清除屏幕
    for(int iy=0;iy<g_ptOledDev->iYres;iy++){
        for(int ix = 0;ix < g_ptOledDev->iXres;ix++){
           // printf("(%d,%d)",ix,iy);
            g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,0);
        }          
    }
	
    while(1){
		//RUNNING状态
		if(ptGame->eStatus == GAME_RUNNING){
            //关闭自动刷新,手动刷新,自动刷新会闪烁
            TIMER_FLUSH(DISABLE);
            
			/* 遍历ROLE */
			ptGame->ptIterator->traversal_excute_item(ptGame->ptIterator);
			
			//增加分数
			ptGame->iCurrentScore++;
			
			//增加LEVEL
			//ptGame->iLevel = 35 - (gamedino->game_score)/200;
//			if(gamedino->game_score > 4000){
//				gamedino->game_level = 15;
//			}

			/* 显示分数等 */
			GameShow(ptGame);
            
            /* 手动刷新 */
            g_ptOledDev->flush(g_ptOledDev);
            delay(600);
           // printf("当前分数为%d.\r\n",ptGame->iCurrentScore);
		}
		//STOP状态
		else{
			/* 显示失败画面 */
			ShowGraphInDisplayDev(g_ptOledDev,50,40,&g_tGraphRESTART);
			ShowTextInDisplayDev(g_ptOledDev,10,40,"GAME");
			ShowTextInDisplayDev(g_ptOledDev,86,40,"OVER");
            
            //打开自动刷新
            TIMER_FLUSH(ENABLE);
		}
        
        
        //游戏控制测试
        //从环形缓冲区取出数据
        if(GetInputBuffer(&event) == 0){
            printf("按键产生输入事件----------\r\n");
            printf("按键类型:%d\r\n",event.etype);
			printf("按键值:%d\r\n",event.iKey);
			printf("按键事件:%d\r\n",event.iPressure);
            if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                GameControl(ptGame,GAME_CMD_ROLE_CONTROL);
            }   
            
            if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                GameControl(ptGame,GAME_CMD_RESTART);
            } 
        }
	}
    
}

