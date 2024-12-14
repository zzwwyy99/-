#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "oled_command.h"
#include "show_graph.h"

static unsigned int GetGraphPixelColorFromFrameBuffer(GraphBitMap *ptGraphBitMap, int iX, int iY)
{
	/* 仅适用于16*8点阵 */
	int x,y;
	unsigned char *byte = ptGraphBitMap->pbuffer + ptGraphBitMap->index;
	//printf("index的值是%d.\r\n", ptGraphBitMap->index);
    
	//像素点(iX,iY)在字符点阵中的位置为(x,y)
	x = iX - ptGraphBitMap->iLeftTopX;
	y = iY - ptGraphBitMap->iLeftTopY;
	
	/* 取出对应byte */
	byte += (y / 8)*ptGraphBitMap->iWidth + x;

	/*取出对应位*/
	y = y % 8;

	if(*byte & (1<<y))
        return 1;
    else
        return 0;
}

/* 将位图里的点阵传输到Display设备的显存,即显示出来 */
static void DrawGraphBitMapOnFrameBuffer(PDisplayDevice ptDispalyDev,GraphBitMap *ptGraphBitMap)
{
	int x,y;
	unsigned int dwcolor;

	/* 遍历位图 */
	for(y = ptGraphBitMap->iLeftTopY; y<ptGraphBitMap->iLeftTopY + ptGraphBitMap->iHeight; y++){
		for(x = ptGraphBitMap->iLeftTopX; x<ptGraphBitMap->iLeftTopX + ptGraphBitMap->iWidth; x++){
            
			/* 从位图中取出每个像素的值 */
			dwcolor = GetGraphPixelColorFromFrameBuffer(ptGraphBitMap,x,y);
			//printf("(%d,%d)%d.",x,y,dwcolor);

			ptDispalyDev->SetPixel(ptDispalyDev,x,y,dwcolor);
		}
	}
}

 //因为考虑到有多种DisplayDev,所以单独抽象出这一层
void  ShowGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap)
{
	ptGraphBitMap->iCurOriginX = iX;
	ptGraphBitMap->iCurOriginY = iY;
	
	//得到左上角坐标,使用之前需要初始化ptGraphBitMap中的图片宽度和图片高度
	ptGraphBitMap->iLeftTopX = iX;
	ptGraphBitMap->iLeftTopY = iY - ptGraphBitMap->iHeight + 1;
	
	DrawGraphBitMapOnFrameBuffer(ptDispalyDev,ptGraphBitMap);
}

static void ClearGraphBitMapOnFrameBuffer(PDisplayDevice ptDispalyDev,GraphBitMap *ptGraphBitMap)
{
	int x,y;
    
	/* 遍历位图 */
	for(y = ptGraphBitMap->iLeftTopY; y<ptGraphBitMap->iLeftTopY + ptGraphBitMap->iHeight; y++){
		for(x = ptGraphBitMap->iLeftTopX; x<ptGraphBitMap->iLeftTopX + ptGraphBitMap->iWidth; x++){
			ptDispalyDev->SetPixel(ptDispalyDev,x,y,0);
		}
	}
}


//图片
void  ClearGraphInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,GraphBitMap *ptGraphBitMap)
{
    //在DMA传输时,不擦除显存中的内容,这样的话就不闪烁了
    //大部分时间OLED_I2C1_DMA_BUSYFLAG都等于1.所以可能会浪费很多时间在等待
    //＊＊＊＊＊等待DMA传输完成
    WAIT_FOR_I2CDMA_BUSY();
    
	ptGraphBitMap->iCurOriginX = iX;
	ptGraphBitMap->iCurOriginY = iY;
	
	//得到左上角坐标,使用之前需要初始化ptGraphBitMap中的图片宽度和图片高度
	ptGraphBitMap->iLeftTopX = iX;
	ptGraphBitMap->iLeftTopY = iY - ptGraphBitMap->iHeight + 1;
	
	ClearGraphBitMapOnFrameBuffer(ptDispalyDev,ptGraphBitMap);
}

//清除特定位置
void  ClearInDisplayDev(PDisplayDevice ptDispalyDev,int ix,int iy,int width,int height)
{
    int x,y;
    
    //＊＊＊＊＊等待DMA传输完成
    WAIT_FOR_I2CDMA_BUSY();
    
    /* 遍历位图 */
	for(y = iy - height+1 ; y<iy; y++){
		for(x = ix; x < ix+width; x++){
			//ptDispalyDev->SetPixel(ptDispalyDev,x,y,1);
			ptDispalyDev->SetPixel(ptDispalyDev,x,y,1);
		}
	}
}

