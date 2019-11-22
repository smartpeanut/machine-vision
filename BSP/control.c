#include "stm32f10x.h"
#include "usart.h"
#include "usart2.h"
#include "led.h"
#include "motor.h"
#include "control.h"
#include "delay.h"

extern int flag;
extern int state;
extern char from_phone;
void control()
{
	//USART1_IRQHandler();  
	USART2_IRQHandler();
	if(from_phone == '0')
	{
		USART1_IRQHandler();
		if(flag == 1)
		{
			RGB_MODEL_0();
			jingzhi();
		}
		else if((flag == 0 && state == 0))
		{
			RGB_MODEL_1();
			qianjin();
		}
		else if((flag == 0 && state == 1))
		{
			RGB_MODEL_2();
			qianjin();
			delay_ms(1200);
			zuozhuan();
			delay_ms(4200);
		}
		else if((flag == 0 && state == 2))
		{
			RGB_MODEL_3();
			RGB_MODEL_2();
			qianjin();
			delay_ms(1200);
			youzhuan();
			delay_ms(4200);
		}	
		else 
		{
			RGB_MODEL_7();
			jingzhi();
		}
	}
	else
	{
		if(from_phone == '1')
		{
			RGB_MODEL_4();
			qianjin();
		}
		else if(from_phone == '2')
		{
			RGB_MODEL_5();
			houtui();
		}
		else if(from_phone == '3')
		{
			RGB_MODEL_6();
			zuozhuan();
		}
		else if(from_phone == '4')
		{
			RGB_MODEL_6();
			youzhuan();
		}
		else
		{
			RGB_MODEL_7();
			jingzhi();
		}
	}
	/*
	if(flag == 1)
	{
		RGB_MODEL_1();
		jingzhi();
	}
	else if((flag == 0 && state == 0) || from_phone == '0')
	{
		RGB_MODEL_2();
		qianjin();
	}
	else if(from_phone == '1')
	{
		RGB_MODEL_3();
		houtui();
	}
	else if(from_phone == '2')
	{
		RGB_MODEL_4();
		jingzhi();
	}
	else if((flag == 0 && state == 1) || from_phone == '3')
	{
		RGB_MODEL_5();
		zuozhuan();
	}
	else if((flag == 0 && state == 2) || from_phone == '4')
	{
		RGB_MODEL_6();
		youzhuan();
	}	
	else 
	{
		RGB_MODEL_7();
		jingzhi();
	}
	if(flag == 0)
	{
		switch(state)
		{
			case 0:
				qianjin();
				break;
			case 1:
				zuozhuan();
				break;
			case 2:
				youzhuan();
				break;
			default:
				jingzhi();
				break;
		}
	}	*/
}
