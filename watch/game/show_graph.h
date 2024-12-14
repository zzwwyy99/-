#ifndef __SHOW_GRAPH_H
#define __SHOW_GRAPH_H

#include "display_system.h"

typedef struct GraphBitMap{
	int iLeftTopX;		/* λͼ���Ͻ�X���� */
	int iLeftTopY;		/* λͼ���Ͻ�Y���� */
	int iWidth;			/* ͼƬ��� */
	int iHeight;		/* ͼƬ�߶� */
	int iCurOriginX;	/* ԭ��X���� */
	int iCurOriginY;	/* ԭ��Y���� */
	unsigned char *pbuffer;	/* ͼƬ���� */
	int index; 			/* ͼƬ�����е���ʼλ�� */
}GraphBitMap,*PGraphBitMap;

void  ShowGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap);
void  ClearGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap);
void  ClearInDisplayDev(PDisplayDevice ptDispalyDev,int ix,int iy,int width,int height);
void  ALLClearInDisplayDev(PDisplayDevice ptDispalyDev);

#endif	/*__SHOW_GRAPH_H*/

