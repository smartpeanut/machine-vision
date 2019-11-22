//头文件
#include "usart.h"
#include "motor.h"
#include "stm32f10x_usart.h"
#include "led.h"
#include "usart2.h"
//=============================================================================
//函数名称:USART1_Config
//功能概要:USART1 GPIO 配置,工作模式配置。115200 8-N-1
//参数说明:无
//函数返回:无
//=============================================================================
void USART1_Config(void)
{	
    GPIO_InitTypeDef GPIO_InitStructure;	
    USART_InitTypeDef USART_InitStructure;  //定义串口初始化结构体
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	//本函数（使能时钟）参数中，RCC_APB2Periph_USART1是必不可少的，有人会问，对于串口用到的PA9和
	//PA10不用使能时钟吗？其实由于USART1默认的就是PA9和PA10，所以这一个就行了，当然你要是加上
	//这个|RCC_APB2Periph_GPIOA也是不报错的，只是重复了。
	
	/*  USART1_TX -> PA9 */			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	       //选中串口默认输出管脚         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //定义输出最大速率 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//定义管脚9的模式  
	GPIO_Init(GPIOA, &GPIO_InitStructure);           //调用函数，把结构体参数输入进行初始化		   
	/*  USART1_RX ->	PA10*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位 无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//使能接收和发送引脚

	USART_Init(USART1, &USART_InitStructure); //将以上赋完值的结构体带入库函数USART_Init进行初始化
	USART_ClearFlag(USART1,USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	USART_Cmd(USART1, ENABLE);//开启USART1，注意与上面RCC_APB2PeriphClockCmd()设置的区别
}



//=============================================================================
//函数名称:fputc
//功能概要:重定向c库函数printf到USART
//参数说明:无
//函数返回:无
//注意   :由printf调用
//=============================================================================
int fputc(int ch, FILE *f)
{
//将Printf内容发往串口 
  USART_SendData(USART1, (unsigned char) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
  return (ch);
}
//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意   :无
//=============================================================================
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

//=============================================================================
//函数名称:USART1_Putc
//功能概要:将USART1_Putc（）内容打印到串口
//参数说明:无
//函数返回:无
//注意    :无
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
		if(temp != '#')   //未接受到结束标志前，存储所有数据
		{
			USART_RXBUF[count] = temp;
			++count;
		}
		if(temp == '#')   //接收到结束标志后，进行数据处理
		{
			if(USART_RXBUF[0] == 's')   //起始标志验证
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
			//道路模式的判断，flag是停止标志，state是左转右转标志
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
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIO端口使能  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//串口端口使能  
      
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
      
    USART_Cmd(USART1,ENABLE);//使能串口1  
      
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启接收中断  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART1_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
      
}  
