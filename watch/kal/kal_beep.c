#include "cal_beep.h" 
#include "beep_device.h"

void KAL_HardwareBeepDevInit(BeepDevice *ptBeepDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_HardwareBeepDevInit(ptBeepDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
}

void KAL_BeepDevicePWMCfg(BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse)
{
	/*对于裸机: 使用CAL*/
    period = 72000 / period - 1; 
	CAL_BeepDevicePWMCfg(ptBeepDevice,period,pulse);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
	//rt_pwm_set(pwm_device, BEEP_PWM_CH, period, pulse);//channel,period,pulse
}

void KAL_BeepDevicePWMOn(BeepDevice *ptBeepDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_BeepDevicePWMOn(ptBeepDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
	//rt_pwm_enable(pwm_device, BEEP_PWM_CH); //使能蜂鸣器对应的 PWM 通道
}

void KAL_BeepDevicePWMOff(BeepDevice *ptBeepDevice)
{
	/*对于裸机: 使用CAL*/
	CAL_BeepDevicePWMOff(ptBeepDevice);

	/*对于rtthread: 使用rtthread提供的驱动框架*/
	//rt_pwm_disable(pwm_device, BEEP_PWM_CH); //失能蜂鸣器对应的 PWM 通道
}

