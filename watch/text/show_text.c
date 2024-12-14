#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "display_device.h"
#include "fontlib_system.h"
#include "encode_str.h"

static unsigned int GetPixelColorFromFrameBuffer(FontBitMap *ptFontBitMap, int iX, int iY)
{
	/* 仅适用于16*8点阵 */
	int x,y;
	unsigned char *byte = ptFontBitMap->pbuffer;
	
	//像素点(iX,iY)在字符点阵中的位置为(x,y)
	x = iX - ptFontBitMap->iLeftTopX;
	y = iY - ptFontBitMap->iLeftTopY;

	if(y<8){
		/* 取出对应byte */
		byte += x;
	}else{
		byte += 8 + x;
	}

    if(y >= 8){
        /*取出对应位*/
		y -= 8;
    }
	 if(*byte & (1<<y))
        return 1;
     else
        return 0;
}

/*如果没有在DMA中断中加入I2C_GenerateSTOP(I2C1,ENABLE);
 *　１.printf("(%d,%d)%d.",x,y,dwcolor);后就没有oled上没有字符显示．这是因为在第一遍flush的时候,显示缓存中写入了字符串"ask100",
 *       按理来说应该等到第二遍flush的时候才刷新.而且还没来得及写完,这说明a.DMA传输时,必须用信号量保护显示缓存 b.cpu速度远大于I2C速度
 *  2.在写完字符串需要flush的时候,因为没有如果没有在DMA中断中加入I2C_GenerateSTOP,所以I2C一直是busy状态
 */
/* 将位图里的点阵传输到Display设备的显存,即显示出来 */
static void DrawBitMapOnFrameBuffer(PDisplayDevice ptDispalyDev,FontBitMap *ptFontBitMap)
{
	int x,y;
	unsigned int dwcolor;

	/* 遍历位图 */
	for(y = ptFontBitMap->iLeftTopY; y<ptFontBitMap->iLeftTopY + ptFontBitMap->iHeight; y++){
		for(x = ptFontBitMap->iLeftTopX; x<ptFontBitMap->iLeftTopX + ptFontBitMap->iWidth; x++){
            
			/* 从位图中取出每个像素的值 */
			dwcolor = GetPixelColorFromFrameBuffer(ptFontBitMap,x,y);
			//printf("(%d,%d)%d.",x,y,dwcolor);
			ptDispalyDev->SetPixel(ptDispalyDev,x,y,dwcolor);
		}
	}
}

/*字符串显示步骤:
 * 1.按照默认解码方式解码该字符串中的字符,比如按照ascii,utf8
 * 2.由取出的dwcode到默认字库取得字符点阵,并保存到位图中.
 * 3.将位图中的字符点阵,输入到显示设备的显存中
 */
 //因为考虑到有多种DisplayDev,所以单独抽象出这一层
void  ShowTextInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,char *str)
{
	int i=0;
	
	//定义一个位图
	FontBitMap tFontBitMap;
	unsigned int dwcode;
	
	tFontBitMap.iCurOriginX = iX;
	tFontBitMap.iCurOriginY = iY;

	while(str[i]){
		//取出一个字符,解码
		EncodeStr(&str[i],&dwcode);
		i++;
	
        tFontBitMap.pbuffer = 0;
		//取出字符点阵:需要先初始化iCurOriginX,iCurOriginY
		if(GetFontBitMap(dwcode,&tFontBitMap) == 0){
			/* 将位图里的点阵传输到Display设备的显存 */
			DrawBitMapOnFrameBuffer(ptDispalyDev,&tFontBitMap);
		}

		tFontBitMap.iCurOriginX = tFontBitMap.iNextOriginX;
		tFontBitMap.iCurOriginY = tFontBitMap.iNextOriginY;
        
	}

	//ptDispalyDev->flush(ptDispalyDev);
}

static char *reverse(char *s)
{
	char temp;
	char *p = s;//p指向s的头部
	char *q = s;//q指向s的尾部
	while(*q){
		++q;
	}
	
	q--;
	//交换指针
	while(q>p){
		temp = *p;
		*p++  =*q;
		*q-- = temp;
	}
	return s;
}

//还有printf形式的打印函数
void ShowNumberInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,int Num)
{
	int i = 0;
	static char s[20];

    do{
		s[i++] = Num % 10 + '0';
		Num = Num /10;
	}while(Num > 0);

	s[i] = '\0';
	
	ShowTextInDisplayDev(ptDispalyDev,iX,iY,reverse(s));
}
