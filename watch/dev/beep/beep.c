#include <stm32f10x.h>

#include "beep_device.h"
#include "kal_beep.h"

/* 宏定义 */
#define	PWM_PIN			GPIO_Pin_8 
#define PWM_GPIO_PORT	GPIOB
#define PWM_TIMER		TIM4


//初始化dev
static void BeepDeviceInit(BeepDevice *ptBeepDevice)
{
	KAL_HardwareBeepDevInit(ptBeepDevice);
}

static void BeepDevicePWMCfg(struct BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse)
{
	KAL_BeepDevicePWMCfg(ptBeepDevice,period,pulse);
}

static void BeepDevicePWMOn(struct BeepDevice *ptBeepDevice)
{
	
	KAL_BeepDevicePWMOn(ptBeepDevice);
}

static void BeepDevicePWMOff(struct BeepDevice *ptBeepDevice)
{
	KAL_BeepDevicePWMOff(ptBeepDevice);

}

static BeepDevice g_tBeepDevice = {
	.iPin = GPIO_Pin_8,
	.iPort = GPIOB,
	.timer = TIM4,
	
	.init = BeepDeviceInit,
	.pwm_cfg = BeepDevicePWMCfg,
	.pwm_on = BeepDevicePWMOn,
	.pwm_off = BeepDevicePWMOff,
};

void AddBeepDeviceBeep(void)
{
	BeepDeviceRegister(&g_tBeepDevice,"beep");
	//关闭蜂鸣器
	g_tBeepDevice.pwm_off(&g_tBeepDevice);
}

