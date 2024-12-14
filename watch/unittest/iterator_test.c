#include <stm32f10x.h>
#include <stdio.h>

#include "show_graph.h"
#include "drive_delay.h"
#include "role_manager.h"
#include "role_system.h"

extern struct Iterator g_tIterator;

void iterator_test(void)
{	
	int i = 0;
	
	//显示设备已经初始化
	printf("iterator test:\r\n");

	//添加role
	AddRole();

    while(1){
		i++;
        delay(600);
        g_tIterator.traversal_excute_item(&g_tIterator);

		if(i == 30){
			DefaultDinosaurJUMP();
            
            i=0;
		}
    }
	
}

