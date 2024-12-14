#include <stdio.h>

#include "decode.h"

#define SEMIBREVE_LEN 1600
#define SOUND_SPACE   4/5
#define SOUND_SIGNATURE       2    //调号：升0个半音
#define SOUND_OCTACHORD       1   //升降八度：升一个八度

static const int freq_table[12] = {262, 277, 294, 311, 330, 349, 369, 392, 415, 440, 466, 494};	//频率表 CDEFGAB
static const int sign_table[7]  = {0, 2, 4, 5, 7, 9, 11}; 	//1~7在频率表中的位置
static const int length_tab[7] = {1, 2, 4, 8, 16, 32, 64};  //几分音符 2^0,2^1,2^2,2^3...
static int freq_table_new[12];	//新的频率表


//signature|调号(0-11)       :  是指乐曲升多少个半音演奏;
//octachord|升降八度(-2到+2) :  < 0 降几个八度; > 0 升几个八度
static int BeepSongNewFreqTable(int signature, int octachord)
{
	int i=0,j=0;

	/* 根据调号及升降八度来生成新的频率表 */
	for(i=0;i<12;i++){
		j = i + signature;	
		
		/* 升完之后超出本组基本音级，就跳到下一组音级 */
		if(j > 11){	
			j = j - 12;
			freq_table_new[i] = freq_table[j] * 2;
		}else{
			freq_table_new[i] = freq_table[j];
		}

		/* 升降八度 */
		if(octachord < 0){
			freq_table_new[i] >>= (-octachord);
		}else{
			freq_table_new[i] <<= octachord;
		}
	}

	return 0;
}

static int BeepSongDecode(unsigned int tone, unsigned int length, unsigned int *freq, unsigned int *sound_len,unsigned int *nosound_len)
{
    static const unsigned int  div0_len = SEMIBREVE_LEN;        // 全音符的长度(ms)
    unsigned int  note_len, note_sound_len, current_freq;
    unsigned int  note, sharp, range, note_div, effect, dotted;

    note = tone % 10;                             //计算出音符
    range = tone / 10 % 10;                       //计算出高低音
    sharp = tone / 100;                           //计算出是否升半
    current_freq = freq_table_new[sign_table[note - 1] + sharp]; //查出对应音符的频率
    if (note != 0) {
        if (range == 1) current_freq >>= 1;       //低音 降八度
        if (range == 3) current_freq <<= 1;       //高音 升八度
        *freq = current_freq;
    }else{
        *freq = 0;
    }
//	printf("频率是%d.\r\n",current_freq);
	
    note_div = length_tab[length % 10];           //算出是几分音符
    effect = length / 10 % 10;                    //算出音符类型(0普通1连音2顿音)
    dotted = length / 100;                        //算出是否附点
    note_len = div0_len / note_div;               //算出音符的时长

    if (dotted == 1)
        note_len = note_len + note_len / 2;

	if(effect == 1){							 //算出普通音符的演奏长度
		note_sound_len = note_len;
	}else if(effect == 0){						 //算出顿音的演奏长度
		note_sound_len = note_len * SOUND_SPACE;
	}else{
		note_sound_len = note_len;				 //算出连音的演奏长度
	}
	
    if (note == 0)
    {
        note_sound_len = 0;
    }
    *sound_len = note_sound_len;
    *nosound_len = note_len - note_sound_len;	 //算出不发音的长度
//	printf("节拍是%d.",note_sound_len);
//	printf("非节拍是%d.\r\n",(note_len - note_sound_len));
	
    return 0;
}


int GetBeepSongLen(const struct BeepSong*ptBeepSong)
{
    //局部变量必须初始化
    //ZI段不需要初始化,但局部变量需要
	int iLen = 0;

	/* 歌曲以0x00 0x00结尾 检测结束标志*/
	while(ptBeepSong->data[iLen]){
		iLen += 2;
	}

	return iLen/2;
}


void GetBeepSongName(const struct BeepSong*ptBeepSong,char *name)
{
	int i=0;

	while(ptBeepSong->name[i]){
		name[i] = ptBeepSong->name[i];
		i++;
	}
	name[i] = '\0';
}

int BeepSongGetData(const struct BeepSong *ptBeepSong, int index, struct BeepSongData *ptdata)
{
    BeepSongDecode(ptBeepSong->data[index * 2],ptBeepSong->data[index * 2+1],&ptdata->freq,&ptdata->sound_len,&ptdata->nosound_len);

    return 0;
}

int BeepSongDecodeInit(void)
{
    BeepSongNewFreqTable(SOUND_SIGNATURE, SOUND_OCTACHORD);  // 0, 1
    printf("音乐解码器初始化成功.\r\n");
    
    return 0;
}


