//头文件
#include "led.h"

//=============================================================================
//函数名称: RGB_GPIO_Config(void)
//功能概要:RGB灯引脚配置
//对应引脚：PA4 -> R, PA5 -> G, PA6->B 
//参数名称:无
//函数返回:无
//=============================================================================
void RGB_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//使能GPIOC的外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	///设置引脚模式为推免输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//设置引脚速度为50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//调用库函数，初始化GPIO
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void RGB_MODEL_0()
{
	R_OFF;
	G_OFF;
	B_OFF;
}
void RGB_MODEL_1()
{
	R_ON;
	G_OFF;
	B_OFF;
}
void RGB_MODEL_2()
{
	R_OFF;
	G_ON;
	B_OFF;
}
void RGB_MODEL_3()
{
	R_OFF;
	G_OFF;
	B_ON;
}
void RGB_MODEL_4()
{
	R_ON;
	G_ON;
	B_OFF;
}
void RGB_MODEL_5()
{
	R_ON;
	G_OFF;
	B_ON;
}
void RGB_MODEL_6()
{
	R_OFF;
	G_ON;
	B_ON;
}
void RGB_MODEL_7()
{
	R_ON;
	G_ON;
	B_ON;
}
//void LED_GPIO_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;

//	RCC_APB2PeriphClockCmd(LED_CLK,ENABLE);
//	GPIO_InitStruct.GPIO_Pin =LED_Pin;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(LED_Port,&GPIO_InitStruct);
//}
void LED_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 				 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


	
