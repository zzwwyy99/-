#include <stdio.h>

#include "show_text.h"
#include "role_manager.h"
#include "game_system.h"

struct Game g_tGame;

extern struct Iterator g_tIterator;

PGame __GetGame(void)
{
	static int inited = 0;
	
	/* 已经初始化 */
	if(inited){
		return 0;
	}
	g_tGame.eStatus = GAME_RUNNING;
	g_tGame.iHighestScore = 0;
	g_tGame.iCurrentScore = 0;
	g_tGame.iLevel = 0;
	g_tGame.ptIterator = &g_tIterator;
	inited = 1;

	return &g_tGame;
}
