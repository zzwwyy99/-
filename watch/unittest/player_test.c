#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"
#include "beep_system.h"
#include "led_system.h"
#include "input_system.h"
#include "input_buffer.h"
#include "player_system.h"
#include "player.h"



void player_test(void)
{
    InputEvent event;
	PPlayer ptPlayerDev;
    int buffer[PLAYER_BUFFER_SIZE], size; //20
    int volume = 50;
    
	//��ʼ��LedSystem,��ʼ��BeepSystem	
	ADDBeepDevice();
	AddLedDevice();
   	ptPlayerDev =  PlayerInit();

    while (1)
    {
        if (ptPlayerDev->status == PLAYER_RUNNING)
        {
            size = ptPlayerDev->iSongTimeAll - ptPlayerDev->iSongTimePass;
            if (size > PLAYER_BUFFER_SIZE)
				size = PLAYER_BUFFER_SIZE;
            size = ptPlayerDev->decode->read(ptPlayerDev->SongSheet[ptPlayerDev->iSongCurrent - 1], ptPlayerDev->iSongTimePass, buffer, size);
            if (size > 0)
            {
                ptPlayerDev->audio->write(buffer, size);
                ptPlayerDev->iSongTimePass += size;
            }
            /* �������ʱ�䵽�ˣ��л�����һ�� */
            if (ptPlayerDev->iSongTimePass >= ptPlayerDev->iSongTimeAll)
            {
                PlayerNext(ptPlayerDev);
                //PlayerShow(ptPlayerDev);
            }
            
            //������Ϣ
            PlayerShow(ptPlayerDev);
        }
        else
        {
            /* ��ͣ����ʱ�ر���Ƶ�豸*/
            ptPlayerDev->audio->close();

                
            printf("����ֹͣ.\r\n");
            /* �ȴ����ŵ��ź��� */
           // rt_sem_take(player->sem_play, RT_WAITING_FOREVER);

            /* ��ʼ����ʱ����Ƶ�豸*/
            //ptPlayerDev->audio->open();
        }
        
        
        //���Ʋ���
        if(GetInputBuffer(&event) == 0){
//          printf("�������������¼�----------\r\n");
//          printf("��������:%d\r\n",event.etype);
//			printf("����ֵ:%d\r\n",event.iKey);
//			printf("�����¼�:%d\r\n",event.iPressure);
			//����һ:last ������:stop ������:next
            
            //����Ӹ�delay����MenuThreadEntryִ��ʱ��ܳ�,��û��ʵʱ��.���԰����������ó���
            //delay(10000);
            if(event.iKey == 1 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                PlayerControl(ptPlayerDev,PLAYER_CMD_LAST,&volume);
            }   
            else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                PlayerControl(ptPlayerDev,PLAYER_CMD_STOP,&volume);
            } 
			else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
				PlayerControl(ptPlayerDev,PLAYER_CMD_NEXT,&volume);
			}
			else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
                volume--;
				PlayerControl(ptPlayerDev,PLAYER_CMD_SET_VOL,&volume);
			}
        }
    }
}

