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

/* λͼ */
extern GraphBitMap g_tGraphRESTART;
/* λͼ */
extern GraphBitMap g_tGraphSTART;

void game_test(void)
{
    InputEvent event;
	PGame ptGame;

	ptGame = GetGame();
    
    AddRole();
    
    printf("game test:\r\n");
    /* ��ʾ�������� */
	ShowGraphInDisplayDev(g_ptOledDev,0,64,&g_tGraphSTART);
	delay(15000);
	//�����Ļ
	//�����Ļ
    for(int iy=0;iy<g_ptOledDev->iYres;iy++){
        for(int ix = 0;ix < g_ptOledDev->iXres;ix++){
           // printf("(%d,%d)",ix,iy);
            g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,0);
        }          
    }
	
    while(1){
		//RUNNING״̬
		if(ptGame->eStatus == GAME_RUNNING){
            //�ر��Զ�ˢ��,�ֶ�ˢ��,�Զ�ˢ�»���˸
            TIMER_FLUSH(DISABLE);
            
			/* ����ROLE */
			ptGame->ptIterator->traversal_excute_item(ptGame->ptIterator);
			
			//���ӷ���
			ptGame->iCurrentScore++;
			
			//����LEVEL
			//ptGame->iLevel = 35 - (gamedino->game_score)/200;
//			if(gamedino->game_score > 4000){
//				gamedino->game_level = 15;
//			}

			/* ��ʾ������ */
			GameShow(ptGame);
            
            /* �ֶ�ˢ�� */
            g_ptOledDev->flush(g_ptOledDev);
            delay(600);
           // printf("��ǰ����Ϊ%d.\r\n",ptGame->iCurrentScore);
		}
		//STOP״̬
		else{
			/* ��ʾʧ�ܻ��� */
			ShowGraphInDisplayDev(g_ptOledDev,50,40,&g_tGraphRESTART);
			ShowTextInDisplayDev(g_ptOledDev,10,40,"GAME");
			ShowTextInDisplayDev(g_ptOledDev,86,40,"OVER");
            
            //���Զ�ˢ��
            TIMER_FLUSH(ENABLE);
		}
        
        
        //��Ϸ���Ʋ���
        //�ӻ��λ�����ȡ������
        if(GetInputBuffer(&event) == 0){
            printf("�������������¼�----------\r\n");
            printf("��������:%d\r\n",event.etype);
			printf("����ֵ:%d\r\n",event.iKey);
			printf("�����¼�:%d\r\n",event.iPressure);
            if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                GameControl(ptGame,GAME_CMD_ROLE_CONTROL);
            }   
            
            if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                GameControl(ptGame,GAME_CMD_RESTART);
            } 
        }
	}
    
}

