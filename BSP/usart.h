#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void USART1_IRQHandler(void);	
int fputc(int ch, FILE *f);
void USART1_Putc(u8 c);
void USART1_Puts(char * str);
void USART1_IRQHandler(void);
void USART1_Init(void);
typedef enum 
{
    FALSE = 0,
    TRUE = 1
} bool;

#endif