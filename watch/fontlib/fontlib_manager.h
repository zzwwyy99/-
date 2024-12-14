#ifndef __FONTLIB_MANAGER_H
#define __FONTLIB_MANAGER_H

#ifndef NULL
#define NULL (void *)0
#endif

typedef struct FontBitMap{
	int iLeftTopX;		/* 位图左上角X坐标 */
	int iLeftTopY;		/* 位图左上角Y坐标 */
	int iWidth;			/* 字体宽度 */
	int iHeight;		/* 字体高度 */
	int iCurOriginX;	/* 原点X坐标 */
	int iCurOriginY;	/* 原点X坐标 */
	int iNextOriginX;	/* 下一个字符X坐标 */
	int iNextOriginY;	/* 下一个字符X坐标 */
	unsigned char *pbuffer;	/* 字符点阵 */
}FontBitMap,*PFontBitMap;

/* fontlib结构体 */
typedef struct FontLib{
	char *name;  		/* 设备名 */
	
	void (*init)(struct FontLib *ptFontLib);				/* 初始化字库 */
	void (*get_font_size)(int *piWidth, int *piHeigh);		/* 获取该字库中字体大小 */
	int  (*set_font_size)(int iFontSize);         			/* 设置字体大小 */
	int  (*get_font_bitmap)(unsigned int dwCode,PFontBitMap ptFontMap);		/* 将字库中的数据写入到字体位图 */
}FontLib,*PFontLib;

void FontLibRegister(struct FontLib *ptFontLib,char *name);
PFontLib __GetFontLib(char *name);

#endif	/*__FONTLIB_MANAGER_H*/

