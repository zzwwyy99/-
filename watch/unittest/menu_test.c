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
	
	printf("menu_test开始.\r\n");

	InitMenuOption();
	while(1){
		MenuThreadEntry(NULL);

		//控制测试
        if(GetInputBuffer(&event) == 0){
//          printf("按键产生输入事件----------\r\n");
//          printf("按键类型:%d\r\n",event.etype);
//			printf("按键值:%d\r\n",event.iKey);
//			printf("按键事件:%d\r\n",event.iPressure);
			//按键一:back 按键二:back 按键三:enter
            
            //如果加个delay或者MenuThreadEntry执行时间很长,就没有实时性.所以按键检测必须拿出来
            //而且如果多按了的话,会保存在ring_buffer里面,就会有延迟的效果
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

