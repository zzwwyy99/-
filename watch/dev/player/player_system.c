#include <stdio.h>
#include <string.h>
#include <stm32f10x.h>

#include "message_manager.h"
#include "input_system.h"
#include "player.h"
#include "player_system.h"
#include "drive_delay.h"

//��һ��������ǲ���������̨,�����ٳ����һ��

int PlayerPlay(Player *ptPlayer)
{
	if(ptPlayer->status != PLAYER_RUNNING){
		ptPlayer->status = PLAYER_RUNNING;
	}

	/* �����ź��� */
	
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
	
	/* ���²��Ÿ��� */
	if(ptPlayer->iSongCurrent > 1){
		ptPlayer->iSongCurrent -= 1;
	}else{
		ptPlayer->iSongCurrent = ptPlayer->iSongNum;
	}

	/* ���²���������ʱ�� */
	ptPlayer->iSongTimePass = 0;

	/* ��õ�ǰ�����ĳ��� */
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_LEN,&iLen);

	/* �����ܲ���ʱ�� */
	ptPlayer->iSongTimeAll = iLen;

	if(ptPlayer->status != PLAYER_RUNNING){
		PlayerPlay(ptPlayer);
	}
	
	return 0;
}		

int PlayerNext(Player *ptPlayer)
{
	int iLen;
	
	/* ���²��Ÿ��� */
	if(ptPlayer->iSongCurrent < ptPlayer->iSongNum){
		ptPlayer->iSongCurrent += 1;
	}else{
		ptPlayer->iSongCurrent = 1;
	}

	/* ���²���������ʱ�� */
	ptPlayer->iSongTimePass = 0;

	/* ��õ�ǰ�����ĳ��� */
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_LEN,&iLen);

	/* �����ܲ���ʱ�� */
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
	
	/* ��ӡ�赥 */
	for(i=0;i < ptPlayer->iSongNum;i++){
		ptPlayer->decode->control(ptPlayer->SongSheet[i],DECODE_OPS_CMD_GET_NAME,name);
		printf("%d:%s\r\n",i,name);
	}

	/* ��ӡ��ǰ����״̬ */
	if(ptPlayer->status == PLAYER_RUNNING){
		printf("<----- ���ڲ���:");
	}else{
		printf("<----- ��ͣ����:");
	}
	
	/* ��ӡ��ǰ�������� */
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_NAME,name);
	printf("%s",name);
	printf(" ---->\r\n");

	/* ��ӡ���Ž��� */
	iPercent = ptPlayer->iSongTimePass * 100 / ptPlayer->iSongTimeAll;
	printf("���Ž���:%d%% ����:%d%%\r\n.",iPercent,ptPlayer->iVolume);

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

/*������ǰ����豸ע�ắ��:register */
int PlayerStart(Player *ptPlayer)
{
	int iLen;
	static int inited = 0;
	
	/* �Ѿ���ʼ�� */
	if(inited){
		return -1;
	}

	/* ����Ƿ��и��� */
	if(ptPlayer->iSongNum == 0){
		return -1;
	}

	/* ���ýӿڳ�ʼ������� */
	ptPlayer->decode->init();

	ptPlayer->status  = PLAYER_STOP;
	ptPlayer->iVolume = PLAYER_SOUND_SIZE_DEFAULT;
	ptPlayer->iSongCurrent = 1;
	ptPlayer->iSongTimePass = 0;
	ptPlayer->decode->control(ptPlayer->SongSheet[ptPlayer->iSongCurrent - 1],DECODE_OPS_CMD_GET_LEN,&iLen);
	ptPlayer->iSongTimeAll = iLen;

	/* ���ýӿڳ�ʼ����Ƶ�豸 */
	ptPlayer->audio->init();
	ptPlayer->audio->control(AUDIO_OPS_CMD_SET_VOL,&ptPlayer->iVolume);

	inited = 1;
	return 0;
}


//���Ͳ�����״̬��ҵ����ϵͳ
//��Ϣ����
int player_status = PLAYER_RUNNING;

struct msg_topic g_tMsgPlayer = {
	.name = "player_topic",
	.msg_data = &player_status
};

//������
void PlayerPublish(void *arg)
{
	//������Ϣ�����߳�
	CoreProcss(arg);
}

struct Publisher g_tPublisherPlayer = {
	.Publish = PlayerPublish
};


void PlayerThreadInit(void)
{
	printf("Player Thread Init.\r\n");
    
   	g_ptPlayerDev =  PlayerInit();
  
    //ע��MSG
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
            /* �������ʱ�䵽�ˣ��л�����һ�� */
            if (g_ptPlayerDev->iSongTimePass >= g_ptPlayerDev->iSongTimeAll)
            {
                PlayerNext(g_ptPlayerDev);
                //PlayerShow(ptPlayerDev);
            }
            
            //������Ϣ
            PlayerShow(g_ptPlayerDev);   
        }
        else
        {
            /* ��ͣ����ʱ�ر���Ƶ�豸*/
            g_ptPlayerDev->audio->close();

            printf("����ֹͣ.\r\n");
            delay(10000);
           
            /* �ȴ����ŵ��ź��� */
           // rt_sem_take(player->sem_play, RT_WAITING_FOREVER);

            /* ��ʼ����ʱ����Ƶ�豸*/
            //ptPlayerDev->audio->open();
        }
        
        //����msg��ҵ����ϵͳ
        player_status = g_ptPlayerDev->status;
        g_tPublisherPlayer.Publish(&g_tMsgPlayer);
    }
}

