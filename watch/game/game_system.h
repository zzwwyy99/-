#ifndef __GAME_SYSTEM_H
#define __GAME_SYSTEM_H

#include "role_manager.h"

enum GAME_STATUS{
	GAME_RUNNING,				//运行状态
	GAME_FAIlED					//失败状态
};

enum GAME_CMD
{
    GAME_CMD_ROLE_CONTROL,
    GAME_CMD_RESTART,
    GAME_CMD_BACK
};
	
typedef struct Game{
	enum GAME_STATUS eStatus;
	int iHighestScore; 		//最高分
	int iCurrentScore;  	//当前分
	int iLevel;				//难度等级:延时时间 15-35
	//int iCharacterNum;	dino数量
	
	struct Iterator *ptIterator;
}Game,*PGame;

int GameRunning(struct Game *ptGame);
int GameStop(struct Game *ptGame);
int GameControl(Game *ptGame,int iCmd);
int DefaultGameControl(int iCmd);
void GameShow(struct Game *ptGame);
PGame GetGame(void);
void GameThreadEntry(void *arg);
void GameThreadInit(void);

#endif	/*__GAME_SYSTEM_H*/

