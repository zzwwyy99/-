#include <stm32f10x.h>

#include "led_device.h"
#include "kal_led.h"

// WHITE
#define LED_WHITE_GPIO_PORT	GPIOA			  		/* GPIO�˿� */
#define LED_WHITE_GPIO_CLK	RCC_APB2Periph_GPIOA	/* GPIO�˿�ʱ�� */
#define LED_WHITE_GPIO_PIN	GPIO_Pin_5				/* GPIO_Pin*/

// RED
#define LED_RED_GPIO_PORT    	GPIOB			            	/* GPIO�˿� */
#define LED_RED_GPIO_CLK 	    RCC_APB2Periph_GPIOB			/* GPIO�˿�ʱ�� */
#define LED_RED_GPIO_PIN		GPIO_Pin_0			       		/* ���ӵ�SCLʱ���ߵ�GPIO */

//��ʼ��LED
static void LedDeviceInit(LedDevice *ptLedDevice)
{
	KAL_GPIOLedDevInit(ptLedDevice);
}

//����LED����,��ת
static void LedDeviceControl(LedDevice *ptLedDevice,int istatus)
{
	KAL_GPIOLedDevControl(ptLedDevice,istatus);
}


static LedDevice LED_WHITE = {
	.iPin = LED_WHITE_GPIO_PIN,
	.iPort = LED_WHITE_GPIO_PORT,
	.init = LedDeviceInit,
	.control = LedDeviceControl
};

static LedDevice LED_RED = {
	.iPin = LED_RED_GPIO_PIN,
	.iPort = LED_RED_GPIO_PORT,
	.init = LedDeviceInit,
	.control = LedDeviceControl
};


void AddLedDeviceToManager(void)
{
	LedDeviceRegister(&LED_WHITE,"WHITE");
    LedDeviceRegister(&LED_RED,"RED");
}

