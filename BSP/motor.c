#include "motor.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

void MOTOR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(MOTOR_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = MOTOR_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_13;
	GPIO_Init(MOTOR_Port,&GPIO_InitStruct);
}

void qianjin()
{
	MOTOR1_GO_1;
	MOTOR1_BACK_0;
	MOTOR2_GO_1;
	MOTOR2_BACK_0;
	MOTOR3_GO_1;
	MOTOR3_BACK_0;
	MOTOR4_GO_1;
	MOTOR4_BACK_0;
}

void houtui()
{
	MOTOR1_GO_0;
	MOTOR1_BACK_1;
	MOTOR2_GO_0;
	MOTOR2_BACK_1;
	MOTOR3_GO_0;
	MOTOR3_BACK_1;
	MOTOR4_GO_0;
	MOTOR4_BACK_1;
}

void zuozhuan()
{
	MOTOR1_GO_0;
	MOTOR1_BACK_1;
	MOTOR2_GO_1;
	MOTOR2_BACK_0;
	MOTOR3_GO_0;
	MOTOR3_BACK_1;
	MOTOR4_GO_1;
	MOTOR4_BACK_0;
}

void youzhuan()
{
	MOTOR1_GO_1;
	MOTOR1_BACK_0;
	MOTOR2_GO_0;
	MOTOR2_BACK_1;
	MOTOR3_GO_1;
	MOTOR3_BACK_0;
	MOTOR4_GO_0;
	MOTOR4_BACK_1;
}

void jingzhi()
{
	MOTOR1_GO_0;
	MOTOR1_BACK_0;
	MOTOR2_GO_0;
	MOTOR2_BACK_0;
	MOTOR3_GO_0;
	MOTOR3_BACK_0;
	MOTOR4_GO_0;
	MOTOR4_BACK_0;
}

