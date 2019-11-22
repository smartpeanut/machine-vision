//ͷ�ļ�
#include "usart.h"
#include "motor.h"
#include "stm32f10x_usart.h"
#include "led.h"
#include "usart2.h"
//=============================================================================
//��������:USART1_Config
//���ܸ�Ҫ:USART1 GPIO ����,����ģʽ���á�115200 8-N-1
//����˵��:��
//��������:��
//=============================================================================
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //���崮�ڳ�ʼ���ṹ��
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	//��������ʹ��ʱ�ӣ������У�RCC_APB2Periph_USART1�Ǳز����ٵģ����˻��ʣ����ڴ����õ���PA9��
	//PA10����ʹ��ʱ������ʵ����USART1Ĭ�ϵľ���PA9��PA10��������һ�������ˣ���Ȼ��Ҫ�Ǽ���
	//���|RCC_APB2Periph_GPIOAҲ�ǲ�����ģ�ֻ���ظ��ˡ�
	
	/*  USART1_TX -> PA9 */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	       //ѡ�д���Ĭ������ܽ�         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�������������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ܽ�9��ģʽ  
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //���ú������ѽṹ�����������г�ʼ��		   
	/*  USART1_RX ->	PA10*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//У��λ ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ʹ�ܽ��պͷ�������

	USART_Init(USART1, &USART_InitStructure); //�����ϸ���ֵ�Ľṹ�����⺯��USART_Init���г�ʼ��
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	USART_Cmd(USART1, ENABLE);//����USART1��ע��������RCC_APB2PeriphClockCmd()���õ�����
}



//=============================================================================
//��������:fputc
//���ܸ�Ҫ:�ض���c�⺯��printf��USART
//����˵��:��
//��������:��
//ע��   :��printf����
//=============================================================================
int fputc(int ch, FILE *f)
{
//��Printf���ݷ������� 
  USART_SendData(USART1, (unsigned char) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
  return (ch);
}
//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��   :��
//=============================================================================
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//��������:USART1_Putc
//���ܸ�Ҫ:��USART1_Putc�������ݴ�ӡ������
//����˵��:��
//��������:��
//ע��    :��
//=============================================================================
void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

unsigned int USART_RXBUF[18];
char line_type;
int flag,state;
void USART1_IRQHandler(void)  
{  
	char type[2];
	static uint8_t count = 0;
    u8 temp;  
	int cx_mean, cx, cy;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		temp=USART_ReceiveData(USART1);
		if(temp != '#')   //δ���ܵ�������־ǰ���洢��������
		{
			USART_RXBUF[count] = temp;
			++count;
		}
		if(temp == '#')   //���յ�������־�󣬽������ݴ���
		{
			if(USART_RXBUF[0] == 's')   //��ʼ��־��֤
			{
				cx_mean = 100*(USART_RXBUF[1]-'0') + 10*(USART_RXBUF[2]-'0') + (USART_RXBUF[3]-'0');
				cx      = 100*(USART_RXBUF[4]-'0') + 10*(USART_RXBUF[5]-'0') + (USART_RXBUF[6]-'0');
				cy      = 100*(USART_RXBUF[7]-'0') + 10*(USART_RXBUF[8]-'0') + (USART_RXBUF[9]-'0');
				//speed   = 100*(USART_RXBUF[10]-'0')+ 10*(USART_RXBUF[11]-'0')+ (USART_RXBUF[12]-'0');
				if(USART_RXBUF[13] == '1')
					line_type = 'C';
				else if(USART_RXBUF[12] == '1')
					line_type = 'T';
				else if(USART_RXBUF[11] == '1' && USART_RXBUF[10] == '0')
					line_type = 'R';
				else if(USART_RXBUF[10] == '1' && USART_RXBUF[11] == '0')
					line_type = 'L';
				else
					line_type = 'N';
			}
			if(cx_mean <= 60)
				youzhuan();
			if(cx_mean >= 260)
				youzhuan();
			count = 0;
			flag = 1;
			//��·ģʽ���жϣ�flag��ֹͣ��־��state����ת��ת��־
			if(line_type == 'C' && (cy > 80))
			{
				LED1_ON;
				flag = 1;
				state = 0;
			}
			else if(line_type == 'T' && (cy > 80))
			{
				LED1_ON;
				flag = 1;
				state = 0;
			}
			else if(line_type == 'L')
			{
				LED1_OFF;
				flag = 0;
				state = 1;
			}
			else if(line_type == 'R')
			{
				LED1_OFF;
				flag = 0;
				state = 2;
			}
			else if(line_type == 'N')
			{
				LED1_OFF;
				flag = 0;
				state = 0;
			}
			*type = line_type;
			if(type[0] != type[1])
				USART_SendData(USART2,type[1]);
			//USART_SendData(USART1,line_type);	
			//USART_SendData(USART2,line_type);			
		}

	}  
} 

void USART1_Init(void)  
{  
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO�˿�ʹ��  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//���ڶ˿�ʹ��  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOA,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=9600;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART1,&USART_InitStrue);
      
    USART_Cmd(USART1,ENABLE);//ʹ�ܴ���1  
      
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���������ж�  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
