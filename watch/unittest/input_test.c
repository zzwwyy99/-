#include <stm32f10x.h>
#include <stdio.h>

#include "button_system.h"
#include "input_system.h"
#include "input_buffer.h"

void input_test(void)
{
    /* 0x080011E8 &Region$$Table$$Limit */
    //InputEvent *pev;  //使用指针的初始值是0x080011E8
    InputEvent event;
    
    //pev = &event; 如果没有初始化指针,指针初始值为0x080011E8
    //*pev = event; GetInputBuffer(pev)中执行*ptInputEvent = g_tInputEventBuffer.buffer[g_tInputEventBuffer.pRead]; \
                        相当于没有初始化指针pev,就给指针pev指向的地址赋值,也就是向0x080011E8赋值
    //指针做参数时,子函数大多数时需要向指针指向的地址赋值,此时指针必须已经初始化.
    
	//初始化按键设备
	AddButtonDevice();
    
	//初始化输入设备
    AddInputDevice();
    InitInputDevice();
    
    printf("input_test\r\n");
    
	//测试按键队列输入,输出
    #if 0
	while(1){
        while(GetInputBuffer(&event) == 0){
            printf("按键产生输入事件----------\r\n");
            printf("按键类型:%d\r\n",event.etype);
			printf("按键值:%d\r\n",event.iKey);
			printf("按键事件:%d\r\n",event.iPressure);
        }
    }
    #endif
}

