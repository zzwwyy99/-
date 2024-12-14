#include <stm32f10x.h>
#include "button_device.h"

//��������
#define KEY1_PIN    GPIO_Pin_6
#define KEY2_PIN    GPIO_Pin_1
#define KEY3_PIN    GPIO_Pin_8

//�����¼��ص�����
static my_button_callback g_fButtonCallBack = 0;

void SetButtonInputDeviceCallBack(my_button_callback func)
{
    g_fButtonCallBack = func;

}

void button_cb(void *para)
{	
	/*����InputEvent*/
    if(g_fButtonCallBack){
        g_fButtonCallBack(para);
    }
}

static Button button_1  = {
	.iPressLogicLevel = 0,
	.iHoldCycPeriod = 100,
    
	//.cb = g_fButtonCallBack,
    // �������޷��ڱ���ʱ���һ���ǳ�����ֵ����ֻ��������ʱͨ����ȡ������ַ����ӵõ�������ֵ\
            ��ȫ�ֱ����ڱ���ʱ�ͱ���ȷ����ֵ����C�о�̬�洢�����ݱ����ó�����ʼ���Ĺ涨��
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

//ע�ᰴ��
void AddButtonDeviceKEY(void)
{
    ButtonRegister(&button_1,"KEY1");
    ButtonRegister(&button_2,"KEY2");
    ButtonRegister(&button_3,"KEY3");
}







