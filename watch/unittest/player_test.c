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
    
	//初始化LedSystem,初始化BeepSystem	
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
            /* 如果播放时间到了，切换到下一首 */
            if (ptPlayerDev->iSongTimePass >= ptPlayerDev->iSongTimeAll)
            {
                PlayerNext(ptPlayerDev);
                //PlayerShow(ptPlayerDev);
            }
            
            //更新信息
            PlayerShow(ptPlayerDev);
        }
        else
        {
            /* 暂停播放时关闭音频设备*/
            ptPlayerDev->audio->close();

                
            printf("播放停止.\r\n");
            /* 等待播放的信号量 */
           // rt_sem_take(player->sem_play, RT_WAITING_FOREVER);

            /* 开始播放时打开音频设备*/
            //ptPlayerDev->audio->open();
        }
        
        
        //控制测试
        if(GetInputBuffer(&event) == 0){
//          printf("按键产生输入事件----------\r\n");
//          printf("按键类型:%d\r\n",event.etype);
//			printf("按键值:%d\r\n",event.iKey);
//			printf("按键事件:%d\r\n",event.iPressure);
			//按键一:last 按键二:stop 按键三:next
            
            //如果加个delay或者MenuThreadEntry执行时间很长,就没有实时性.所以按键检测必须拿出来
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

