#include <stdio.h>

#include "fontlib_manager.h"
#include "ascii_fontlib.h"

static PFontLib g_ptFaultFontlib;

void ADDFontLib(void)
{
	/* ���FontLib��FontLib������ */
	AddFontLibASCII();
}

/*���һ��FontLib*/
PFontLib GetFontLib(char *name)
{
	return __GetFontLib(name);
}

//�趨һ��Ĭ���ֿ�
void SetDefaultFontLib(void)
{
	/* ���ASCII�ֿ⵽�ֿ������ */
	ADDFontLib();
	
	g_ptFaultFontlib = GetFontLib("ascii");

	if(g_ptFaultFontlib){
		printf("����Ĭ���ֿ�ASCII�ɹ�.\r\n");
	}else{
		printf("����Ĭ���ֿ�ASCIIʧ��.\r\n");
	}
}


//��Ĭ���ֿ���ȡ���ַ�����
int GetFontBitMap(unsigned int dwcode, PFontBitMap ptFontBitMap)
{  
    return g_ptFaultFontlib->get_font_bitmap(dwcode,ptFontBitMap);
}

