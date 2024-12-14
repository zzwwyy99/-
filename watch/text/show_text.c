#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "display_device.h"
#include "fontlib_system.h"
#include "encode_str.h"

static unsigned int GetPixelColorFromFrameBuffer(FontBitMap *ptFontBitMap, int iX, int iY)
{
	/* ��������16*8���� */
	int x,y;
	unsigned char *byte = ptFontBitMap->pbuffer;
	
	//���ص�(iX,iY)���ַ������е�λ��Ϊ(x,y)
	x = iX - ptFontBitMap->iLeftTopX;
	y = iY - ptFontBitMap->iLeftTopY;

	if(y<8){
		/* ȡ����Ӧbyte */
		byte += x;
	}else{
		byte += 8 + x;
	}

    if(y >= 8){
        /*ȡ����Ӧλ*/
		y -= 8;
    }
	 if(*byte & (1<<y))
        return 1;
     else
        return 0;
}

/*���û����DMA�ж��м���I2C_GenerateSTOP(I2C1,ENABLE);
 *����.printf("(%d,%d)%d.",x,y,dwcolor);���û��oled��û���ַ���ʾ��������Ϊ�ڵ�һ��flush��ʱ��,��ʾ������д�����ַ���"ask100",
 *       ������˵Ӧ�õȵ��ڶ���flush��ʱ���ˢ��.���һ�û���ü�д��,��˵��a.DMA����ʱ,�������ź���������ʾ���� b.cpu�ٶ�Զ����I2C�ٶ�
 *  2.��д���ַ�����Ҫflush��ʱ��,��Ϊû�����û����DMA�ж��м���I2C_GenerateSTOP,����I2Cһֱ��busy״̬
 */
/* ��λͼ��ĵ����䵽Display�豸���Դ�,����ʾ���� */
static void DrawBitMapOnFrameBuffer(PDisplayDevice ptDispalyDev,FontBitMap *ptFontBitMap)
{
	int x,y;
	unsigned int dwcolor;

	/* ����λͼ */
	for(y = ptFontBitMap->iLeftTopY; y<ptFontBitMap->iLeftTopY + ptFontBitMap->iHeight; y++){
		for(x = ptFontBitMap->iLeftTopX; x<ptFontBitMap->iLeftTopX + ptFontBitMap->iWidth; x++){
            
			/* ��λͼ��ȡ��ÿ�����ص�ֵ */
			dwcolor = GetPixelColorFromFrameBuffer(ptFontBitMap,x,y);
			//printf("(%d,%d)%d.",x,y,dwcolor);
			ptDispalyDev->SetPixel(ptDispalyDev,x,y,dwcolor);
		}
	}
}

/*�ַ�����ʾ����:
 * 1.����Ĭ�Ͻ��뷽ʽ������ַ����е��ַ�,���簴��ascii,utf8
 * 2.��ȡ����dwcode��Ĭ���ֿ�ȡ���ַ�����,�����浽λͼ��.
 * 3.��λͼ�е��ַ�����,���뵽��ʾ�豸���Դ���
 */
 //��Ϊ���ǵ��ж���DisplayDev,���Ե����������һ��
void  ShowTextInDisplayDev(PDisplayDevice ptDispalyDev,int iX,int iY,char *str)
{
	int i=0;
	
	//����һ��λͼ
	FontBitMap tFontBitMap;
	unsigned int dwcode;
	
	tFontBitMap.iCurOriginX = iX;
	tFontBitMap.iCurOriginY = iY;

	while(str[i]){
		//ȡ��һ���ַ�,����
		EncodeStr(&str[i],&dwcode);
		i++;
	
        tFontBitMap.pbuffer = 0;
		//ȡ���ַ�����:��Ҫ�ȳ�ʼ��iCurOriginX,iCurOriginY
		if(GetFontBitMap(dwcode,&tFontBitMap) == 0){
			/* ��λͼ��ĵ����䵽Display�豸���Դ� */
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
	char *p = s;//pָ��s��ͷ��
	char *q = s;//qָ��s��β��
	while(*q){
		++q;
	}
	
	q--;
	//����ָ��
	while(q>p){
		temp = *p;
		*p++  =*q;
		*q-- = temp;
	}
	return s;
}

//����printf��ʽ�Ĵ�ӡ����
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
