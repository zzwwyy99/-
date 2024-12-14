#ifndef __PLAYER_SYSTEM_H
#define __PLAYER_SYSTEM_H

#ifndef NULL
#define NULL (void *)0
#endif

#define PLAYER_BUFFER_SIZE 20

#define PLAYER_SOUND_SIZE_DEFAULT 	50
#define PLAYER_SONG_NUM_MAX 	  	10
#define PLAYER_SONG_NAME_LEN_MAX	20

typedef enum{
	PLAYER_RUNNING,		//正在播放
	PLAYER_STOP			//停止播放
}PLAYE_STATUS;

enum PLAYER_CMD
{
    PLAYER_CMD_PLAY,
    PLAYER_CMD_STOP,
    PLAYER_CMD_LAST,
    PLAYER_CMD_NEXT,
    PLAYER_CMD_SET_VOL,
    PLAYER_CMD_GET_VOL,
    PLAYER_CMD_GET_STATUS
};

enum AUDIO_OPS_CMD{
	AUDIO_OPS_CMD_SET_VOL	
};

enum DECODE_OPS_CMD{
	DECODE_OPS_CMD_GET_NAME,
	DECODE_OPS_CMD_GET_LEN,
};

struct audio_ops
{
    int (*init)(void);
    int (*open)(void);
    int (*close)(void);
    int (*control)(int cmd, void *arg);
    int (*write)(void *buffer, int size);
};

struct decode_ops
{
    int (*init)(void);
    int (*control)(void *song, int cmd, void *arg);
    int (*read)(void *song, int index, void *buffer, int size);
};


typedef struct Player{
	PLAYE_STATUS status;		//当前播放状态
	int iVolume;				//声音大小
	int iSongCurrent;			//当前正在播放的歌曲	
	int iSongNum;				//歌曲总数目
	int iSongTimePass;			//已经播放的时间
	int iSongTimeAll;			//总播放时间
	void *SongSheet[PLAYER_SONG_NUM_MAX];			//歌曲列表

#if 0
	rt_sem_t				 sem_play;		  //用于播放状态控制的信号量
	rt_thread_t 			 play_thread;	  //播放的线程
#endif		

	struct audio_ops  *audio;
	struct decode_ops *decode;
}Player,*PPlayer;

int PlayerPlay(Player *ptPlayer);
int PlayerStop(Player *ptPlayer);
int PlayerLast(Player *ptPlayer);
int PlayerNext(Player *ptPlayer);

int PlayerControl(Player *ptPlayer,int iCmd,void *arg);
int DefaultPlayerControl(int iCmd,void *arg);
int PlayerShow(Player *ptPlayer);
int PlayerAddSong(Player *ptPlayers,void *pSong);
int PlayerStart(Player *ptPlayer);
void PlayerThreadInit(void);
void PlayerThreadEntry(void *arg);
#endif	/*__PLAYER_SYSTEM_H*/

