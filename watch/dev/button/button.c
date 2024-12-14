#include <stm32f10x.h>
#include "button_device.h"

//三个按键
#define KEY1_PIN    GPIO_Pin_6
#define KEY2_PIN    GPIO_Pin_1
#define KEY3_PIN    GPIO_Pin_8

//按键事件回调函数
static my_button_callback g_fButtonCallBack = 0;

void SetButtonInputDeviceCallBack(my_button_callback func)
{
    g_fButtonCallBack = func;

}

void button_cb(void *para)
{	
	/*产生InputEvent*/
    if(g_fButtonCallBack){
        g_fButtonCallBack(para);
    }
}

static Button button_1  = {
	.iPressLogicLevel = 0,
	.iHoldCycPeriod = 100,
    
	//.cb = g_fButtonCallBack,
    // 编译器无法在编译时求得一个非常量的值，它只能在运行时通过读取变量地址来间接得到变量的值\
            而全局变量在编译时就必须确定其值，故C有静态存储区数据必须用常量初始化的规定。
    .cb = button_cb,
	.iPin = KEY1_PIN	//64
};

static Button button_2  = {
	.iPressLogicLevel = 0,
	.iHoldCycPeriod = 100,
	.cb = button_cb,
	.iPin = KEY2_PIN	//2
};

static Button button_3  = {
	.iPressLogicLevel = 0,
	.iHoldCycPeriod = 100,
	.cb = button_cb,
	.iPin = KEY3_PIN	//256
};

//注册按键
void AddButtonDeviceKEY(void)
{
    ButtonRegister(&button_1,"KEY1");
    ButtonRegister(&button_2,"KEY2");
    ButtonRegister(&button_3,"KEY3");
}







