/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "delay.h" 
#include "led.h"
#include "motor.h"
#include "usart.h"
#include "usart2.h"
#include "control.h"
/* Private functions ---------------------------------------------------------*/

extern int flag;
extern char from_phone;
int main(void)
{
	USART1_Init();
	USART2_Init(9600);
	LED_GPIO_Config();
	RGB_GPIO_Config();
	MOTOR_GPIO_Config();
    /* Infinite loop */
    while (1)
	{
		//USART1_IRQHandler();  
		//USART2_IRQHandler();
		control();
    }
}

