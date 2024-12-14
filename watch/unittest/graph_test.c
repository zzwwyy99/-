#include <stm32f10x.h>
#include <stdio.h>

#include "show_graph.h"
#include "drive_delay.h"
#include "graph_data.h"

GraphBitMap g_tGraphCloud = {
	.iWidth = 24,
	.iHeight = 8,
	.pbuffer = (unsigned char *)CLOUD
};


GraphBitMap g_tGraphDinosaur = {
	.iWidth  = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)DINOSAUR
};


GraphBitMap g_tGraphCactus_1 = {
	.iWidth = 8,
	.iHeight = 16,
	.pbuffer = (unsigned char *)Cactus1
};
	
GraphBitMap g_tGraphGround = {
	.iWidth = 128,
	.iHeight = 8,
	.pbuffer = (unsigned char *)GroundImg
};

extern PDisplayDevice g_ptOledDev;

void graph_test(void)
{	
	int i=0;
	
	//显示设备已经初始化
	printf("Graph test:\r\n");

	ShowGraphInDisplayDev(g_ptOledDev,10,16,&g_tGraphCloud);
	ShowGraphInDisplayDev(g_ptOledDev,20,30,&g_tGraphDinosaur);
    ShowGraphInDisplayDev(g_ptOledDev,70,30,&g_tGraphDinosaur);
	ShowGraphInDisplayDev(g_ptOledDev,70,55,&g_tGraphCactus_1);

	delay(1000);
    ClearGraphInDisplayDev(g_ptOledDev,10,16,&g_tGraphCloud);
	delay(1000);
	i = 16;
//   
//    while(i<128){
//        ShowGraphInDisplayDev(ptOledDev,0,64,&g_tGraphGround,i++);
//        delay(1000);
//    }
//    printf("450到了.\r\n");  

	//擦除
	while(i<64){
        ClearGraphInDisplayDev(g_ptOledDev,10,i,&g_tGraphCloud);
		ShowGraphInDisplayDev(g_ptOledDev,10,++i,&g_tGraphCloud);
        
        delay(100);
        //ClearGraphInDisplayDev(ptOledDev,10,i-1,&g_tGraphCloud,0);
	}
}

