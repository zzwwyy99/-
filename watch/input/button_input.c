#include <drive_timer.h>
#include <stdio.h>
#include <rtthread.h>

#include "button.h"
#include "input_device.h"
#include "input_buffer.h"

extern struct rt_thread g_tLogicThread;

/* 按键回调函数 */
static void ButtonInputDeviceCallBack(void *arg)
{
	PButton ptemp = (PButton)arg;
	InputEvent event;
	
	event.etype = INPUT_EVENT_TYPE_BUTTON;
	/* switch语句判断是哪个按键 */
    switch(ptemp->iPin){
        case 64:
            event.iKey = 1;
            break;
        case 2:
            event.iKey = 2;
            break;
        case 256:
            event.iKey = 3;
            break;
        default:
            break;
    }
	event.iPressure = ptemp->eEvent;

	/* 放入环形缓冲区*/
	PutInputBuffer(&event);
}

//用定时器2扫描.所以需要初始化定时器2
//定时器不在这里初始化,添加到按键管理器里面了
static int ButtonInputInit(void)
{
	/* Button设备,扫描按键用到的timer,都在button_system中初始化 */

	/* 设定按键的回调函数:button_cb */
	SetButtonInputDeviceCallBack(ButtonInputDeviceCallBack);
    
    return 0;
}

//定义一个button输入设备
static InputDevice g_tButtonInputDevice = {
	"button",
	NULL,
	ButtonInputInit,
	NULL
};

//将button设备注册到设备管理器
void AddButtonInputDevice(void)
{
	InputDeviceRegister(&g_tButtonInputDevice);
}

