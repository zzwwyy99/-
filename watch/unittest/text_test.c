#include <stm32f10x.h>
#include <stdio.h>

#include "drive_delay.h"
#include "display_system.h"
#include "oled_command.h"
#include "fontlib_system.h"
#include "show_text.h"

extern PDisplayDevice g_ptOledDev;
    
void text_test(void)
{
    int ix,iy;
	char *str = "ask10012123";

	//初始化默认显示设备
	g_ptOledDev = SetDefaultDisplayDev();

	if(!g_ptOledDev){
		printf("没有名字为oled的设备.\r\n");
		return;
	}else{
        printf("得到设备:oled.\r\n");
    }

	/* 清除屏幕 */
	oled_clear();
	printf("清除屏幕.\r\n");
    
    //屏幕置一
    for(iy=0;iy<g_ptOledDev->iYres;iy++){
        for(ix = 0;ix < g_ptOledDev->iXres;ix++){
           // printf("(%d,%d)",ix,iy);
            g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,1);
        }          
    }
    
    iy = g_ptOledDev->iYres/2;
    for(ix = 0;ix < g_ptOledDev->iXres;ix++)
        g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,1);
    
    ix = g_ptOledDev->iXres/2;
    for(iy = 0;iy < g_ptOledDev->iYres;iy++)
        g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,1);
    
    for(ix = 0;ix < g_ptOledDev->iXres;ix++){
        iy = ix *2;
        g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,1);
    }
       
     
    for(iy = 0;iy < g_ptOledDev->iYres;iy++){
        ix = (64 - iy) *2;
        g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,1);
    } 
    

    g_ptOledDev->flush(g_ptOledDev);

	/*添加字库,并设定默认字库*/
	printf("添加默认字库ASCII.\r\n");
	SetDefaultFontLib();
		
	/*显示字符串*/
    ShowNumberInDisplayDev(g_ptOledDev,50,32,15030);    
    ShowTextInDisplayDev(g_ptOledDev,12,64,str);
    delay(1000);
    
    //清除屏幕
    for(iy=0;iy<g_ptOledDev->iYres;iy++){
        for(ix = 0;ix < g_ptOledDev->iXres;ix++){
           // printf("(%d,%d)",ix,iy);
            g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,0);
        }          
    }
    
	printf("清除屏幕.\r\n");
    delay(1000);
    
    //帧率测试
    printf("开启定时器刷新屏幕.\r\n");
    TIMER_FLUSH(ENABLE);  
}

