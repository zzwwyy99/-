#include <stm32f10x.h>
#include <stdio.h>

#include "display_system.h"
#include "oled_command.h"

void display_test(void)
{
    int ix,iy;
	//初始化显示设备
	ADDDisplayDevice();
	
	PDisplayDevice ptOledDev = GetDisplayDevice("oled");
	if(!ptOledDev){
		printf("没有名字为oled的设备.\r\n");
		return;
	}

	//注册的时候应该已经初始化了
	//ptdev->init();

	/* 清除屏幕 */
	oled_clear();
	printf("清除屏幕.\r\n");
    
    //画条线
    iy = ptOledDev->iYres/2;
    for(ix = 0;ix < ptOledDev->iXres;ix++)
        ptOledDev->SetPixel(ptOledDev,ix,iy,1);
    
    ix = ptOledDev->iXres/2;
    for(iy = 0;iy < ptOledDev->iYres;iy++)
        ptOledDev->SetPixel(ptOledDev,ix,iy,1);
    
    for(ix = 0;ix < ptOledDev->iXres;ix++){
        iy = ix *2;
        ptOledDev->SetPixel(ptOledDev,ix,iy,1);
    }
       
     
    for(iy = 0;iy < ptOledDev->iYres;iy++){
        ix = (64 - iy) *2;
        ptOledDev->SetPixel(ptOledDev,ix,iy,1);
    }      
    ptOledDev->flush(ptOledDev);
}

