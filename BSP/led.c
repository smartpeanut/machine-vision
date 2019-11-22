//ͷ�ļ�
#include "led.h"

//=============================================================================
//��������: RGB_GPIO_Config(void)
//���ܸ�Ҫ:RGB����������
//��Ӧ���ţ�PA4 -> R, PA5 -> G, PA6->B 
//��������:��
//��������:��
//=============================================================================
void RGB_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ�壬���ֽ�GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//ѡ��Ҫ�õ�GPIO����		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	///��������ģʽΪ�������ģʽ			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	//���������ٶ�Ϊ50MHZ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//���ÿ⺯������ʼ��GPIO
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


	
