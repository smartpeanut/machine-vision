//=============================================================================
//��������:USART2_Config
//���ܸ�Ҫ:USART2 GPIO ����,����ģʽ���á�9600
//����˵��:��
//��������:��
//PA3/RXD2��PA2/TXD2
//=============================================================================
#include "usart2.h"
#include "stm32f10x_usart.h"
#include "usart.h"

void USART2_Init(u32 My_BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	
	// ����ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //��λ����2 -> ����û��
	
	// ��ʼ�� ���ڶ�ӦIO��  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// ��ʼ�� ����ģʽ״̬
	USART_InitStrue.USART_BaudRate=My_BaudRate; // ������
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // Ӳ��������
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // ���� ���� ģʽ��ʹ��
	USART_InitStrue.USART_Parity=USART_Parity_No; // û����żУ��
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // һλֹͣλ
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // ÿ�η������ݿ��Ϊ8λ
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//���������ж�
	
	// ��ʼ�� �ж����ȼ�
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);
}


extern char line_type;
char from_phone;
extern int flag;
void USART2_IRQHandler(void) // ����2�жϷ�����
{
	//extern int cx_mean;
	//u8 res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)) // �жϱ�־
	{
		from_phone = USART_ReceiveData(USART2);  // ����2 ����
		USART_SendData(USART2,line_type);   // ����2 ����
		//flag = 0;
	}
}
