#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"
#include "beep_system.h"
#include "decode.h"

const struct BeepSong song6 = 
{
    .name = "两只老虎",
    .data = { 
        0x15, 0x02, 0x16, 0x02, 0x17, 0x02, 0x15, 0x02, 0x15, 0x02,
        0x16, 0x02, 0x17, 0x02, 0x15, 0x02, 0x17, 0x02, 0x18, 0x02,
        0x19, 0x01, 0x17, 0x02, 0x18, 0x02, 0x19, 0x01, 0x19, 0x03,
        0x1A, 0x03, 0x19, 0x03, 0x18, 0x03, 0x17, 0x02, 0x15, 0x16,
        0x19, 0x03, 0x1A, 0x03, 0x19, 0x03, 0x18, 0x03, 0x17, 0x02,
        0x15, 0x16, 0x15, 0x02, 0x0F, 0x02, 0x15, 0x01, 0x15, 0x02,
        0x0F, 0x02, 0x15, 0x01, 0x00, 0x00 }
};

void decode_test(void)
{
	
    uint16_t freq_tab2[7]  = {523, 587, 659, 699, 784, 880, 988};

    
	PBeepDevice pdev;
	BeepSongData tBeepSongData;
	int i=0,iLen;
    
//	char *name;     又是这个问题,指针没有初始化
//	GetBeepSongName(&song1,name);
    char name[20];
    
	//添加beepdev
    //两次添加会怎么样
	ADDBeepDevice();

	//获得beep设备
	pdev = GetBeepDevice("beep");
	//配置beep_pwm且ON    
    for(int i =0;i<7;i++){
        pdev->pwm_cfg(pdev,freq_tab2[i],50);
    
        delay(1000);
    
        pdev->pwm_off(pdev);
    
        delay(1000);
    
    }
    printf("蜂鸣器测试正常.\r\n");
    
	//初始化解码器
 	BeepSongDecodeInit();

	GetBeepSongName(&song6,name);
	iLen = GetBeepSongLen(&song6);
	
	printf("目前播放歌曲为:%s\r\n",name);
	printf("歌曲长度为:%d\r\n",iLen);

	
	while(i<iLen){
		//解码数据
		BeepSongGetData(&song6,i,&tBeepSongData);

		//将解码后得到tBeepSongData.freq, tBeepSongData.sound_len, tBeepSongData.nosound_len  \
		//经过蜂鸣器播放出去
		pdev->pwm_cfg(pdev,tBeepSongData.freq,70);
		delay(tBeepSongData.sound_len*20);
		
		pdev->pwm_off(pdev);
		delay( tBeepSongData.nosound_len*20);
		
		i++;
	}
}

