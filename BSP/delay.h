

#ifndef _DELAY_H_
#define _DELAY_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

/* Private function prototypes -----------------------------------------------*/
void RTC_Init(void);
void Time_Display(void);
void Time_Regulate(void);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);
/* Private variables ---------------------------------------------------------*/

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
