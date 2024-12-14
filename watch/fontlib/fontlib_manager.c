#include <stdlib.h>
#include <string.h>

#include "fontlib_manager.h"

#define FONTLIB_LIST_MAX	4

/* ����һ��FontLib������ */
typedef struct FontLibManager{
	unsigned int num;
	struct FontLib *fontlib_device_list[FONTLIB_LIST_MAX];
}FontLibManager;

//FontLib������
static FontLibManager g_tFontLibManager;

/*ע�ắ��*/
void FontLibRegister(struct FontLib *ptFontLib,char *name)
{
	/* ��ʼ��FontLib */
	//ptFontLib->init(ptFontLib);
	ptFontLib->name = name;
	
	/*  ���FontLib��FontLib������ */
	g_tFontLibManager.fontlib_device_list[g_tFontLibManager.num++] = ptFontLib;
}

/*���һ��FontLib*/
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

