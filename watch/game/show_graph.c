#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "oled_command.h"
#include "show_graph.h"

static unsigned int GetGraphPixelColorFromFrameBuffer(GraphBitMap *ptGraphBitMap, int iX, int iY)
{
	/* ��������16*8���� */
	int x,y;
	unsigned char *byte = ptGraphBitMap->pbuffer + ptGraphBitMap->index;
	//printf("index��ֵ��%d.\r\n", ptGraphBitMap->index);
    
	//���ص�(iX,iY)���ַ������е�λ��Ϊ(x,y)
	x = iX - ptGraphBitMap->iLeftTopX;
	y = iY - ptGraphBitMap->iLeftTopY;
	
	/* ȡ����Ӧbyte */
	byte += (y / 8)*ptGraphBitMap->iWidth + x;

	/*ȡ����Ӧλ*/
	y = y % 8;

	if(*byte & (1<<y))
        return 1;
    else
        return 0;
}

/* ��λͼ��ĵ����䵽Display�豸���Դ�,����ʾ���� */
static void DrawGraphBitMapOnFrameBuffer(PDisplayDevice ptDispalyDev,GraphBitMap *ptGraphBitMap)
{
	int x,y;
	unsigned int dwcolor;

	/* ����λͼ */
	for(y = ptGraphBitMap->iLeftTopY; y<ptGraphBitMap->iLeftTopY + ptGraphBitMap->iHeight; y++){
		for(x = ptGraphBitMap->iLeftTopX; x<ptGraphBitMap->iLeftTopX + ptGraphBitMap->iWidth; x++){
            
			/* ��λͼ��ȡ��ÿ�����ص�ֵ */
			dwcolor = GetGraphPixelColorFromFrameBuffer(ptGraphBitMap,x,y);
			//printf("(%d,%d)%d.",x,y,dwcolor);

			ptDispalyDev->SetPixel(ptDispalyDev,x,y,dwcolor);
		}
	}
}

 //��Ϊ���ǵ��ж���DisplayDev,���Ե����������һ��
void  ShowGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap)
{
	ptGraphBitMap->iCurOriginX = iX;
	ptGraphBitMap->iCurOriginY = iY;
	
	//�õ����Ͻ�����,ʹ��֮ǰ��Ҫ��ʼ��ptGraphBitMap�е�ͼƬ��Ⱥ�ͼƬ�߶�
	ptGraphBitMap->iLeftTopX = iX;
	ptGraphBitMap->iLeftTopY = iY - ptGraphBitMap->iHeight + 1;
	
	DrawGraphBitMapOnFrameBuffer(ptDispalyDev,ptGraphBitMap);
}

static void ClearGraphBitMapOnFrameBuffer(PDisplayDevice ptDispalyDev,GraphBitMap *ptGraphBitMap)
{
	int x,y;
    
	/* ����λͼ */
	for(y = ptGraphBitMap->iLeftTopY; y<ptGraphBitMap->iLeftTopY + ptGraphBitMap->iHeight; y++){
		for(x = ptGraphBitMap->iLeftTopX; x<ptGraphBitMap->iLeftTopX + ptGraphBitMap->iWidth; x++){
			ptDispalyDev->SetPixel(ptDispalyDev,x,y,0);
		}
	}
}


//ͼƬ
void  ClearGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap)
{
    //��DMA����ʱ,�������Դ��е�����,�����Ļ��Ͳ���˸��
    //�󲿷�ʱ��OLED_I2C1_DMA_BUSYFLAG������1.���Կ��ܻ��˷Ѻܶ�ʱ���ڵȴ�
    //�����������ȴ�DMA�������
    WAIT_FOR_I2CDMA_BUSY();
    
	ptGraphBitMap->iCurOriginX = iX;
	ptGraphBitMap->iCurOriginY = iY;
	
	//�õ����Ͻ�����,ʹ��֮ǰ��Ҫ��ʼ��ptGraphBitMap�е�ͼƬ��Ⱥ�ͼƬ�߶�
	ptGraphBitMap->iLeftTopX = iX;
	ptGraphBitMap->iLeftTopY = iY - ptGraphBitMap->iHeight + 1;
	
	ClearGraphBitMapOnFrameBuffer(ptDispalyDev,ptGraphBitMap);
}

//����ض�λ��
void  ClearInDisplayDev(PDisplayDevice ptDispalyDev,int ix,int iy,int width,int height)
{
    int x,y;
    
    //�����������ȴ�DMA�������
    WAIT_FOR_I2CDMA_BUSY();
    
    /* ����λͼ */
	for(y = iy - height+1 ; y<iy; y++){
		for(x = ix; x < ix+width; x++){
			//ptDispalyDev->SetPixel(ptDispalyDev,x,y,1);
			ptDispalyDev->SetPixel(ptDispalyDev,x,y,1);
		}
	}
}

