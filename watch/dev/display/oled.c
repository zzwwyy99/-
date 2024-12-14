#include <stm32f10x.h>
#include "stdio.h"

#include "display_device.h"
#include "oled_command.h"

static unsigned char *g_OLEDFrameBuffer[1024];
	
//初始化oled
//调用oled_command.c层的函数,这一层里是用i2c做接口,如果有其他的接口可以另写一层
static void OLEDDeviceInit(DisplayDevice *ptDisplayDevice)
{
	//所以不采用KAL的方式,而是直接调用oled_command.c层的函数
    //初始化OLED I2C接口
	oled_init(ptDisplayDevice->FBBase,ptDisplayDevice->iSize);
}


static void OLEDDeviceFlush(DisplayDevice *ptDisplayDevice)
{
	oled_gram_refresh_dma();
}

static int OLEDDeviceSetPixel(struct DisplayDevice *ptDisplayDevice,int iX,int iY,unsigned int dwcolor)
{
	int page,bit;
	unsigned char *byte;
	unsigned char *buf = ptDisplayDevice->FBBase;

    //printf("(%d,%d).",iX,iY);
    //加个小于0的判定条件
	if(iX>=ptDisplayDevice->iXres || iX <= 0 || iY>=ptDisplayDevice->iYres || iY<=0){
		return -1;	
	}
	
	page = iY / 8;
	byte = buf + page*128 + iX;
	bit = iY % 8;

	if(dwcolor){
		*byte |= (1 << bit);	//置1
	}else{
		*byte &= ~(1 << bit);	//清0
	}
    
	return 0;
}

static DisplayDevice g_tOLEDdev = {
	.FBBase = g_OLEDFrameBuffer,
	.iXres = 128,
	.iYres = 64,
	.iBpp = 1,
	.iSize = 128*64*1/8 + 1,
	.init = OLEDDeviceInit,
	.flush = OLEDDeviceFlush,
	.SetPixel = OLEDDeviceSetPixel
};


void AddDisplayDeviceOLED(void)
{
	DisplayDeviceRegister(&g_tOLEDdev,"oled");
}

