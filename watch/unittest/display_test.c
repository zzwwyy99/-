#include <stm32f10x.h>
#include <stdio.h>

#include "display_system.h"
#include "oled_command.h"

void display_test(void)
{
    int ix,iy;
	//��ʼ����ʾ�豸
	ADDDisplayDevice();
	
	PDisplayDevice ptOledDev = GetDisplayDevice("oled");
	if(!ptOledDev){
		printf("û������Ϊoled���豸.\r\n");
		return;
	}

	//ע���ʱ��Ӧ���Ѿ���ʼ����
	//ptdev->init();

	/* �����Ļ */
	oled_clear();
	printf("�����Ļ.\r\n");
    
    //������
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

