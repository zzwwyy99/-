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
	
	//��ʾ�豸�Ѿ���ʼ��
	printf("iterator test:\r\n");

	//���role
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

