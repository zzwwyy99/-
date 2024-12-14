#ifndef __FONTLIB_MANAGER_H
#define __FONTLIB_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct FontBitMap{
	int iLeftTopX;		/* λͼ���Ͻ�X���� */
	int iLeftTopY;		/* λͼ���Ͻ�Y���� */
	int iWidth;			/* ������ */
	int iHeight;		/* ����߶� */
	int iCurOriginX;	/* ԭ��X���� */
	int iCurOriginY;	/* ԭ��X���� */
	int iNextOriginX;	/* ��һ���ַ�X���� */
	int iNextOriginY;	/* ��һ���ַ�X���� */
	unsigned char *pbuffer;	/* �ַ����� */
}FontBitMap,*PFontBitMap;

/* fontlib�ṹ�� */
typedef struct FontLib{
	char *name;  		/* �豸�� */
	
	void (*init)(struct FontLib *ptFontLib);				/* ��ʼ���ֿ� */
	void (*get_font_size)(int *piWidth, int *piHeigh);		/* ��ȡ���ֿ��������С */
	int  (*set_font_size)(int iFontSize);         			/* ���������С */
	int  (*get_font_bitmap)(unsigned int dwCode,PFontBitMap ptFontMap);		/* ���ֿ��е�����д�뵽����λͼ */
}FontLib,*PFontLib;

void FontLibRegister(struct FontLib *ptFontLib,char *name);
PFontLib __GetFontLib(char *name);

#endif	/*__FONTLIB_MANAGER_H*/

