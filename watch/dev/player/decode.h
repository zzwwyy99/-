#ifndef __DECODE_H
#define __DECODE_H

#define SONG_NAME_LENGTH_MAX 30
#define SONG_DATA_LENGTH_MAX 500

typedef struct BeepSong{
	const unsigned char name[SONG_NAME_LENGTH_MAX];
	const unsigned char data[SONG_DATA_LENGTH_MAX];
}BeepSong,*PBeepSong;

typedef struct BeepSongData{
	unsigned int freq;
	unsigned int sound_len;
	unsigned int nosound_len;
}BeepSongData,*PBeepSongData;

int GetBeepSongLen(const struct BeepSong*ptBeepSong);
void GetBeepSongName(const struct BeepSong*ptBeepSong,char *name);
int BeepSongGetData(const struct BeepSong *ptBeepSong, int index, struct BeepSongData *ptdata);
int BeepSongDecodeInit(void);

#endif	/*__DECODE_H*/

