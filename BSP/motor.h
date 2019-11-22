#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

#define       MOTOR3_GO_1   GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define       MOTOR3_GO_0   GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define       MOTOR3_BACK_1 GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define       MOTOR3_BACK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define       MOTOR1_GO_1   GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define       MOTOR1_GO_0   GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define       MOTOR1_BACK_1 GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define       MOTOR1_BACK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_8)

#define       MOTOR2_GO_1   GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define       MOTOR2_GO_0   GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define       MOTOR2_BACK_1 GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define       MOTOR2_BACK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define       MOTOR4_GO_1   GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define       MOTOR4_GO_0   GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define       MOTOR4_BACK_1 GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define       MOTOR4_BACK_0 GPIO_ResetBits(GPIOB, GPIO_Pin_14)

#define	MOTOR_Pin  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
#define MOTOR_Port GPIOB
#define MOTOR_CLK  RCC_APB2Periph_GPIOB

void MOTOR_GPIO_Config(void);
void qianjin(void);
void houtui(void);
void zuozhuan(void);
void youzhuan(void);
void jingzhi(void);

#endif
