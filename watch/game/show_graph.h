#ifndef __SHOW_GRAPH_H
#define __SHOW_GRAPH_H

#include "display_system.h"

typedef struct GraphBitMap{
	int iLeftTopX;		/* 位图左上角X坐标 */
	int iLeftTopY;		/* 位图左上角Y坐标 */
	int iWidth;			/* 图片宽度 */
	int iHeight;		/* 图片高度 */
	int iCurOriginX;	/* 原点X坐标 */
	int iCurOriginY;	/* 原点Y坐标 */
	unsigned char *pbuffer;	/* 图片点阵 */
	int index; 			/* 图片点阵中的起始位置 */
}GraphBitMap,*PGraphBitMap;

void  ShowGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap);
void  ClearGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap);
void  ClearInDisplayDev(PDisplayDevice ptDispalyDev,int ix,int iy,int width,int height);
void  ALLClearInDisplayDev(PDisplayDevice ptDispalyDev);

#endif	/*__SHOW_GRAPH_H*/

