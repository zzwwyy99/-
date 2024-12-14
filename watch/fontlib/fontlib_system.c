#include <stdio.h>

#include "fontlib_manager.h"
#include "ascii_fontlib.h"

static PFontLib g_ptFaultFontlib;

void ADDFontLib(void)
{
	/* 添加FontLib到FontLib管理器 */
	AddFontLibASCII();
}

/*获得一个FontLib*/
PFontLib GetFontLib(char *name)
{
	return __GetFontLib(name);
}

//设定一个默认字库
void SetDefaultFontLib(void)
{
	/* 添加ASCII字库到字库管理器 */
	ADDFontLib();
	
	g_ptFaultFontlib = GetFontLib("ascii");

	if(g_ptFaultFontlib){
		printf("设置默认字库ASCII成功.\r\n");
	}else{
		printf("设置默认字库ASCII失败.\r\n");
	}
}


//从默认字库中取出字符点阵
int GetFontBitMap(unsigned int dwcode, PFontBitMap ptFontBitMap)
{  
    return g_ptFaultFontlib->get_font_bitmap(dwcode,ptFontBitMap);
}

