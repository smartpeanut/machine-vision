#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "stm32f10x.h" 
 
void USART2_Init(u32 My_BaudRate);
void USART2_IRQHandler(void);

#endif
