#include <stdlib.h>
#include <string.h>

#include "fontlib_manager.h"

#define FONTLIB_LIST_MAX	4

/* 定义一个FontLib管理器 */
typedef struct FontLibManager{
	unsigned int num;
	struct FontLib *fontlib_device_list[FONTLIB_LIST_MAX];
}FontLibManager;

//FontLib管理器
static FontLibManager g_tFontLibManager;

/*注册函数*/
void FontLibRegister(struct FontLib *ptFontLib,char *name)
{
	/* 初始化FontLib */
	//ptFontLib->init(ptFontLib);
	ptFontLib->name = name;
	
	/*  添加FontLib到FontLib管理器 */
	g_tFontLibManager.fontlib_device_list[g_tFontLibManager.num++] = ptFontLib;
}

/*获得一个FontLib*/
PFontLib __GetFontLib(char *name)
{
	PFontLib ptFontLib;

	for(int i=0;i<g_tFontLibManager.num;i++){
		ptFontLib = g_tFontLibManager.fontlib_device_list[i];
		if(strcmp(ptFontLib->name,name) == 0){
			return ptFontLib;			
		}
	}

	return NULL;
}

