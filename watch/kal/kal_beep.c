#include "cal_beep.h" 
#include "beep_device.h"

void KAL_HardwareBeepDevInit(BeepDevice *ptBeepDevice)
{
	/*�������: ʹ��CAL*/
	CAL_HardwareBeepDevInit(ptBeepDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
}

void KAL_BeepDevicePWMCfg(BeepDevice *ptBeepDevice,unsigned int period,unsigned int pulse)
{
	/*�������: ʹ��CAL*/
    period = 72000 / period - 1; 
	CAL_BeepDevicePWMCfg(ptBeepDevice,period,pulse);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
	//rt_pwm_set(pwm_device, BEEP_PWM_CH, period, pulse);//channel,period,pulse
}

void KAL_BeepDevicePWMOn(BeepDevice *ptBeepDevice)
{
	/*�������: ʹ��CAL*/
	CAL_BeepDevicePWMOn(ptBeepDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
	//rt_pwm_enable(pwm_device, BEEP_PWM_CH); //ʹ�ܷ�������Ӧ�� PWM ͨ��
}

void KAL_BeepDevicePWMOff(BeepDevice *ptBeepDevice)
{
	/*�������: ʹ��CAL*/
	CAL_BeepDevicePWMOff(ptBeepDevice);

	/*����rtthread: ʹ��rtthread�ṩ���������*/
	//rt_pwm_disable(pwm_device, BEEP_PWM_CH); //ʧ�ܷ�������Ӧ�� PWM ͨ��
}

