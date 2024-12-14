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

	//��ʼ��Ĭ����ʾ�豸
	g_ptOledDev = SetDefaultDisplayDev();

	if(!g_ptOledDev){
		printf("û������Ϊoled���豸.\r\n");
		return;
	}else{
        printf("�õ��豸:oled.\r\n");
    }

	/* �����Ļ */
	oled_clear();
	printf("�����Ļ.\r\n");
    
    //��Ļ��һ
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

	/*����ֿ�,���趨Ĭ���ֿ�*/
	printf("���Ĭ���ֿ�ASCII.\r\n");
	SetDefaultFontLib();
		
	/*��ʾ�ַ���*/
    ShowNumberInDisplayDev(g_ptOledDev,50,32,15030);    
    ShowTextInDisplayDev(g_ptOledDev,12,64,str);
    delay(1000);
    
    //�����Ļ
    for(iy=0;iy<g_ptOledDev->iYres;iy++){
        for(ix = 0;ix < g_ptOledDev->iXres;ix++){
           // printf("(%d,%d)",ix,iy);
            g_ptOledDev->SetPixel(g_ptOledDev,ix,iy,0);
        }          
    }
    
	printf("�����Ļ.\r\n");
    delay(1000);
    
    //֡�ʲ���
    printf("������ʱ��ˢ����Ļ.\r\n");
    TIMER_FLUSH(ENABLE);  
}

