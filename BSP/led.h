#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

//LED0�ǰ�����ɫ�ƣ���B12���ſ��ƣ�LED1�������ƣ���B9���ſ���
#define       LED0_ON   GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define       LED0_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define       LED1_ON   GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define       LED1_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_9)

//R��A4�ڿ��ƣ�G��A5���ƣ�B��A6����
#define       R_ON   GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define       R_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define       G_ON   GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define       G_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define       B_ON   GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define       B_OFF  GPIO_ResetBits(GPIOA, GPIO_Pin_6)

#define	LED_Pin  GPIO_Pin_9 | GPIO_Pin_12
#define LED_Port GPIOB
#define LED_CLK  RCC_APB2Periph_GPIOB

void LED_GPIO_Config(void);	
void RGB_GPIO_Config(void);
void RGB_MODEL_0(void);
void RGB_MODEL_1(void);
void RGB_MODEL_2(void);
void RGB_MODEL_3(void);
void RGB_MODEL_4(void);
void RGB_MODEL_5(void);
void RGB_MODEL_6(void);
void RGB_MODEL_7(void);
#endif
