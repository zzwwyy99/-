#include <stdio.h>

#include "drive_delay.h"
#include "led_system.h"
#include "beep_system.h"
#include "decode.h"
#include "player_system.h"
#include "song_data.h"

static struct audio_ops aduio;
static struct decode_ops decode;

static struct LedDevice *g_ptPlayerPilotLed;
static struct BeepDevice *g_ptPlayerBeepDev;
static struct Player g_tPlayer;

static int beep_volume = 50;

//初始化音频播放模块
static int audio_init(void)
{
	//需要用到beep设备,led设备
	//使用之前,需要初始化BeepSystem,LedSystem
	g_ptPlayerPilotLed = GetLedDevice("RED");

	g_ptPlayerBeepDev = GetBeepDevice("beep");
    return 0;
}

static int audio_open(void)
{
	///开启红灯
	g_ptPlayerPilotLed->control(g_ptPlayerPilotLed,LED_ON);
	
	//蜂鸣器不需要ON
    return 0;
}

static int audio_close(void)
{
	//关闭红灯
	g_ptPlayerPilotLed->control(g_ptPlayerPilotLed,LED_OFF);

	//关闭蜂鸣器
	g_ptPlayerBeepDev->pwm_off(g_ptPlayerBeepDev);
    
    return 0;
}
static int audio_control(int cmd, void *arg)
{
	if(cmd == AUDIO_OPS_CMD_SET_VOL){
		beep_volume = *(int *)arg;
		
		if(beep_volume > 100){
			beep_volume = 100;
		}
		else if(beep_volume < 0){
			beep_volume = 0;
		}
	}
	return beep_volume;
}

static int audio_write(void *buffer, int size)
{
	struct BeepSongData *data = buffer;

	/* 反转灯 */
	g_ptPlayerPilotLed->control(g_ptPlayerPilotLed,LED_TROGGLE);

	g_ptPlayerBeepDev->pwm_cfg(g_ptPlayerBeepDev,data->freq,beep_volume);

	delay(data->sound_len*20);
	
	g_ptPlayerBeepDev->pwm_off(g_ptPlayerBeepDev);
	delay( data->nosound_len*20);

	return size;	
}
	
static int decode_init(void)
{
	BeepSongDecodeInit();
    
    return 0;
}

static int decode_control(void *song, int cmd, void *arg)
{
	if(cmd == DECODE_OPS_CMD_GET_NAME){
		GetBeepSongName(song,arg);
	}else if(cmd == DECODE_OPS_CMD_GET_LEN){
		*(int *)arg = GetBeepSongLen(song);
	}
	
	return 0;
}

static int decode_read(void *song, int index, void *buffer, int size)
{
	BeepSongGetData(song,index,buffer);

	return 1;
}

/* 类似于前面的ADDevice函数　*/
PPlayer PlayerInit(void)
{
	aduio.init = audio_init;
	aduio.open = audio_open;
	aduio.close = audio_close;
	aduio.control = audio_control;
	aduio.write = audio_write;
	
	decode.init = decode_init;
	decode.control = decode_control;
	decode.read = decode_read;
	
	g_tPlayer.audio = &aduio;
	g_tPlayer.decode = &decode;

	/* 添加歌曲 */
	PlayerAddSong(&g_tPlayer,(void*)&song1);
	PlayerAddSong(&g_tPlayer,(void*)&song2);
	PlayerAddSong(&g_tPlayer,(void*)&song3);
	PlayerAddSong(&g_tPlayer,(void*)&song4);
	PlayerAddSong(&g_tPlayer,(void*)&song5);

	PlayerStart(&g_tPlayer);
	PlayerControl(&g_tPlayer,PLAYER_CMD_PLAY,NULL);
	PlayerShow(&g_tPlayer);

	return &g_tPlayer;
}


