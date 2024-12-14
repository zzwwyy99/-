#include <stm32f10x.h>
#include <stdio.h>

#include "graph_data.h"
#include "game_system.h"
#include "drive_delay.h"
#include "show_graph.h"
#include "show_text.h"
#include "role_system.h"
#include "display_system.h"

#include "button_system.h"
#include "input_system.h"
#include "input_buffer.h"

#include "option_manager.h"

	
void menu_test(void)
{
	InputEvent event;
	
	printf("menu_test��ʼ.\r\n");

	InitMenuOption();
	while(1){
		MenuThreadEntry(NULL);

		//���Ʋ���
        if(GetInputBuffer(&event) == 0){
//          printf("�������������¼�----------\r\n");
//          printf("��������:%d\r\n",event.etype);
//			printf("����ֵ:%d\r\n",event.iKey);
//			printf("�����¼�:%d\r\n",event.iPressure);
			//����һ:back ������:back ������:enter
            
            //����Ӹ�delay����MenuThreadEntryִ��ʱ��ܳ�,��û��ʵʱ��.���԰����������ó���
            //��������ఴ�˵Ļ�,�ᱣ����ring_buffer����,�ͻ����ӳٵ�Ч��
            delay(10000);
            if(event.iKey == 1 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                OptionControl(OPTION_CMD_BACK);
            }   
            else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
                OptionControl(OPTION_CMD_NEXT);
            } 
			else if(event.iKey == 3 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 0){
				OptionControl(OPTION_CMD_ENTER);
			}
			else if(event.iKey == 2 && event.etype == INPUT_EVENT_TYPE_BUTTON && event.iPressure == 3){
				OptionControl(OPTION_CMD_NEXT);
			}
        }
	}
}

