#include <stdio.h>

#include "decode.h"

#define SEMIBREVE_LEN 1600
#define SOUND_SPACE   4/5
#define SOUND_SIGNATURE       2    //���ţ���0������
#define SOUND_OCTACHORD       1   //�����˶ȣ���һ���˶�

static const int freq_table[12] = {262, 277, 294, 311, 330, 349, 369, 392, 415, 440, 466, 494};	//Ƶ�ʱ� CDEFGAB
static const int sign_table[7]  = {0, 2, 4, 5, 7, 9, 11}; 	//1~7��Ƶ�ʱ��е�λ��
static const int length_tab[7] = {1, 2, 4, 8, 16, 32, 64};  //�������� 2^0,2^1,2^2,2^3...
static int freq_table_new[12];	//�µ�Ƶ�ʱ�


//signature|����(0-11)       :  ��ָ���������ٸ���������;
//octachord|�����˶�(-2��+2) :  < 0 �������˶�; > 0 �������˶�
static int BeepSongNewFreqTable(int signature, int octachord)
{
	int i=0,j=0;

	/* ���ݵ��ż������˶��������µ�Ƶ�ʱ� */
	for(i=0;i<12;i++){
		j = i + signature;	
		
		/* ����֮�󳬳����������������������һ������ */
		if(j > 11){	
			j = j - 12;
			freq_table_new[i] = freq_table[j] * 2;
		}else{
			freq_table_new[i] = freq_table[j];
		}

		/* �����˶� */
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
    static const unsigned int  div0_len = SEMIBREVE_LEN;        // ȫ�����ĳ���(ms)
    unsigned int  note_len, note_sound_len, current_freq;
    unsigned int  note, sharp, range, note_div, effect, dotted;

    note = tone % 10;                             //���������
    range = tone / 10 % 10;                       //������ߵ���
    sharp = tone / 100;                           //������Ƿ�����
    current_freq = freq_table_new[sign_table[note - 1] + sharp]; //�����Ӧ������Ƶ��
    if (note != 0) {
        if (range == 1) current_freq >>= 1;       //���� ���˶�
        if (range == 3) current_freq <<= 1;       //���� ���˶�
        *freq = current_freq;
    }else{
        *freq = 0;
    }
//	printf("Ƶ����%d.\r\n",current_freq);
	
    note_div = length_tab[length % 10];           //����Ǽ�������
    effect = length / 10 % 10;                    //�����������(0��ͨ1����2����)
    dotted = length / 100;                        //����Ƿ񸽵�
    note_len = div0_len / note_div;               //���������ʱ��

    if (dotted == 1)
        note_len = note_len + note_len / 2;

	if(effect == 1){							 //�����ͨ���������೤��
		note_sound_len = note_len;
	}else if(effect == 0){						 //������������೤��
		note_sound_len = note_len * SOUND_SPACE;
	}else{
		note_sound_len = note_len;				 //������������೤��
	}
	
    if (note == 0)
    {
        note_sound_len = 0;
    }
    *sound_len = note_sound_len;
    *nosound_len = note_len - note_sound_len;	 //����������ĳ���
//	printf("������%d.",note_sound_len);
//	printf("�ǽ�����%d.\r\n",(note_len - note_sound_len));
	
    return 0;
}


int GetBeepSongLen(const struct BeepSong*ptBeepSong)
{
    //�ֲ����������ʼ��
    //ZI�β���Ҫ��ʼ��,���ֲ�������Ҫ
	int iLen = 0;

	/* ������0x00 0x00��β ��������־*/
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
    printf("���ֽ�������ʼ���ɹ�.\r\n");
    
    return 0;
}


