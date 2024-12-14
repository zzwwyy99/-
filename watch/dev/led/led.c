#include <stm32f10x.h>

#include "led_device.h"
#include "kal_led.h"

// WHITE
#define LED_WHITE_GPIO_PORT	GPIOA			  		/* GPIO端口 */
#define LED_WHITE_GPIO_CLK	RCC_APB2Periph_GPIOA	/* GPIO端口时钟 */
#define LED_WHITE_GPIO_PIN	GPIO_Pin_5				/* GPIO_Pin*/

// RED
#define LED_RED_GPIO_PORT    	GPIOB			            	/* GPIO端口 */
#define LED_RED_GPIO_CLK 	    RCC_APB2Periph_GPIOB			/* GPIO端口时钟 */
#define LED_RED_GPIO_PIN		GPIO_Pin_0			       		/* 连接到SCL时钟线的GPIO */

//初始化LED
static void LedDeviceInit(LedDevice *ptLedDevice)
{
	KAL_GPIOLedDevInit(ptLedDevice);
}

//控制LED开关,反转
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

