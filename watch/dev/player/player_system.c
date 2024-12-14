#include <stdio.h>
#include <string.h>
#include <stm32f10x.h>

#include "message_manager.h"
#include "input_system.h"
#include "player.h"
#include "player_system.h"
#include "drive_delay.h"

//这一层可以算是播放器控制台,可以再抽象出一层

int PlayerPlay(Player *ptPlayer)
{
	if(ptPlayer->status != PLAYER_RUNNING){
		ptPlayer->status = PLAYER_RUNNING;
	}

	/* 发送信号量 */
	
	return 0;
}	

int PlayerStop(Player *ptPlayer)
{
	if(ptPlayer->status != PLAYER_STOP){
		ptPlayer->status = PLAYER_STOP;
	}
	
	return 0;
}	

int PlayerLast(Player *ptPlayer)
{
	int iLen;
	
	/* 更新播放歌曲 */
	if(ptPlayer->iSongCurrent > 1){
		ptPlayer->iSongCurrent -= 1;
	}else{
		ptPlayer->iSongCurrent = ptPlayer->iSongNum;
	}

	/* 更新播放器播放时间 */
	ptPlayer->iSongTimePass = 0;

	/* 获得当前歌曲的长度 */
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_LEN,&iLen);

	/* 更新总播放时间 */
	ptPlayer->iSongTimeAll = iLen;

	if(ptPlayer->status != PLAYER_RUNNING){
		PlayerPlay(ptPlayer);
	}
	
	return 0;
}		

int PlayerNext(Player *ptPlayer)
{
	int iLen;
	
	/* 更新播放歌曲 */
	if(ptPlayer->iSongCurrent < ptPlayer->iSongNum){
		ptPlayer->iSongCurrent += 1;
	}else{
		ptPlayer->iSongCurrent = 1;
	}

	/* 更新播放器播放时间 */
	ptPlayer->iSongTimePass = 0;

	/* 获得当前歌曲的长度 */
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_LEN,&iLen);

	/* 更新总播放时间 */
	ptPlayer->iSongTimeAll = iLen;

	if(ptPlayer->status != PLAYER_RUNNING){
		PlayerPlay(ptPlayer);
	}
	
	return 0;
}

int PlayerControl(Player *ptPlayer,int iCmd,void *arg)
{	
	switch(iCmd){
		case PLAYER_CMD_PLAY:
			PlayerPlay(ptPlayer);
			break;
		case PLAYER_CMD_STOP:
			PlayerStop(ptPlayer);
			break;
		case PLAYER_CMD_LAST:
			PlayerLast(ptPlayer);
			break;
		case PLAYER_CMD_NEXT:
			PlayerNext(ptPlayer);
			break;
		case PLAYER_CMD_SET_VOL:
			ptPlayer->iVolume = *(int *)arg;
			ptPlayer->audio->control(AUDIO_OPS_CMD_SET_VOL,&ptPlayer->iVolume);
			break;
		case PLAYER_CMD_GET_VOL:
        	*(int *)arg = ptPlayer->iVolume;
        	break;	
		case PLAYER_CMD_GET_STATUS:
        	*(int *)arg = ptPlayer->status;
        	break;	
	}
	return 0;
}

static PPlayer g_ptPlayerDev;

int DefaultPlayerControl(int iCmd,void *arg)
{	
	switch(iCmd){
		case PLAYER_CMD_PLAY:
			PlayerPlay(g_ptPlayerDev);
			break;
		case PLAYER_CMD_STOP:
			PlayerStop(g_ptPlayerDev);
			break;
		case PLAYER_CMD_LAST:
			PlayerLast(g_ptPlayerDev);
			break;
		case PLAYER_CMD_NEXT:
			PlayerNext(g_ptPlayerDev);
			break;
		case PLAYER_CMD_SET_VOL:
			g_ptPlayerDev->iVolume = *(int *)arg;
			g_ptPlayerDev->audio->control(AUDIO_OPS_CMD_SET_VOL,&g_ptPlayerDev->iVolume);
			break;
		case PLAYER_CMD_GET_VOL:
        	*(int *)arg = g_ptPlayerDev->iVolume;
        	break;	
		case PLAYER_CMD_GET_STATUS:
        	*(int *)arg = g_ptPlayerDev->status;
        	break;	
	}
	return 0;
}


int PlayerShow(Player *ptPlayer)
{
	char name[PLAYER_SONG_NAME_LEN_MAX + 1];
	int i;
	int iPercent;
	
	/* 打印歌单 */
	for(i=0;i < ptPlayer->iSongNum;i++){
		ptPlayer->decode->control(ptPlayer->SongSheet[i],DECODE_OPS_CMD_GET_NAME,name);
		printf("%d:%s\r\n",i,name);
	}

	/* 打印当前播放状态 */
	if(ptPlayer->status == PLAYER_RUNNING){
		printf("<----- 正在播放:");
	}else{
		printf("<----- 暂停播放:");
	}
	
	/* 打印当前歌曲名字 */
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_NAME,name);
	printf("%s",name);
	printf(" ---->\r\n");

	/* 打印播放进度 */
	iPercent = ptPlayer->iSongTimePass * 100 / ptPlayer->iSongTimeAll;
	printf("播放进度:%d%% 音量:%d%%\r\n.",iPercent,ptPlayer->iVolume);

	printf("***********************************\r\n");

	return 0;
}

int PlayerAddSong(Player *ptPlayers,void *pSong)
{
	if(ptPlayers->iSongNum == PLAYER_SONG_NUM_MAX){
		return -1;
	}
	ptPlayers->SongSheet[ptPlayers->iSongNum++] = pSong;

	return 0;
}

/*类似于前面的设备注册函数:register */
int PlayerStart(Player *ptPlayer)
{
	int iLen;
	static int inited = 0;
	
	/* 已经初始化 */
	if(inited){
		return -1;
	}

	/* 检测是否有歌曲 */
	if(ptPlayer->iSongNum == 0){
		return -1;
	}

	/* 调用接口初始化解调器 */
	ptPlayer->decode->init();

	ptPlayer->status  = PLAYER_STOP;
	ptPlayer->iVolume = PLAYER_SOUND_SIZE_DEFAULT;
	ptPlayer->iSongCurrent = 1;
	ptPlayer->iSongTimePass = 0;
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_LEN,&iLen);
	ptPlayer->iSongTimeAll = iLen;

	/* 调用接口初始化音频设备 */
	ptPlayer->audio->init();
	ptPlayer->audio->control(AUDIO_OPS_CMD_SET_VOL,&ptPlayer->iVolume);

	inited = 1;
	return 0;
}


//发送播放器状态给业务子系统
//消息数据
int player_status = PLAYER_RUNNING;

struct msg_topic g_tMsgPlayer = {
	.name = "player_topic",
	.msg_data = &player_status
};

//发布者
void PlayerPublish(void *arg)
{
	//唤醒消息中心线程
	CoreProcss(arg);
}

struct Publisher g_tPublisherPlayer = {
	.Publish = PlayerPublish
};


void PlayerThreadInit(void)
{
	printf("Player Thread Init.\r\n");
    
   	g_ptPlayerDev =  PlayerInit();
  
    //注册MSG
    RegisterMessage("player_topic");
}

void PlayerThreadEntry(void *arg)
{
    int buffer[PLAYER_BUFFER_SIZE], size; //20

	printf("Player Thread.\r\n");    
    while (1)
    {
        if (g_ptPlayerDev->status == PLAYER_RUNNING)
        {            
            size = g_ptPlayerDev->iSongTimeAll - g_ptPlayerDev->iSongTimePass;
            if (size > PLAYER_BUFFER_SIZE)
				size = PLAYER_BUFFER_SIZE;
            size = g_ptPlayerDev->decode->read(g_ptPlayerDev->SongSheet[g_ptPlayerDev->iSongCurrent - 1], g_ptPlayerDev->iSongTimePass, buffer, size);
            if (size > 0)
            {
                g_ptPlayerDev->audio->write(buffer, size);
                g_ptPlayerDev->iSongTimePass += size;
            }
            /* 如果播放时间到了，切换到下一首 */
            if (g_ptPlayerDev->iSongTimePass >= g_ptPlayerDev->iSongTimeAll)
            {
                PlayerNext(g_ptPlayerDev);
                //PlayerShow(ptPlayerDev);
            }
            
            //更新信息
            PlayerShow(g_ptPlayerDev);   
        }
        else
        {
            /* 暂停播放时关闭音频设备*/
            g_ptPlayerDev->audio->close();

            printf("播放停止.\r\n");
            delay(10000);
           
            /* 等待播放的信号量 */
           // rt_sem_take(player->sem_play, RT_WAITING_FOREVER);

            /* 开始播放时打开音频设备*/
            //ptPlayerDev->audio->open();
        }
        
        //发送msg到业务子系统
        player_status = g_ptPlayerDev->status;
        g_tPublisherPlayer.Publish(&g_tMsgPlayer);
    }
}

