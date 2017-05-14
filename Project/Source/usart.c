#include "sys.h"
#include "usart.h"	  
#include "uart_service.h"

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
u8 USART1_RX_STA = 0;       //接收状态标记	  
u8 usart1_len = 0;

#endif


#if EN_USART3_RX

u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART3_RX_STA = 0;              //接收状态标记
u8 usart3_len = 0;

#endif


static uart_t uart_driver = {

	NULL,
	NULL
};

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //使能USART3，GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	USART_DeInit(USART1);  //复位串口1
	USART_DeInit(USART3);  //复位串口3
	
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART3_TX  PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
 
	
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

	//USART3_RX  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	
#if EN_USART1_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif


#if EN_USART3_RX
	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断

#endif

    USART_Cmd(USART1, ENABLE);                    //使能串口 1
	USART_Cmd(USART3, ENABLE);                    //使能串口 3

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); 

	/*call back function*/
	uart_driver.uart1_call_back = uart1_irq_service;
	uart_driver.uart3_call_back = uart3_irq_service;

}







//串口1中断服务程序
void USART1_IRQHandler(void)                	
	{
	u8 Res;
  static u16 i = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		if(i < USART_REC_LEN ){
			USART1_RX_BUF[i++] = Res;
			usart1_len++;
		}
		else{
			i = 0;
			usart1_len = 0;
		}
		if(Res == 0x0d)
			USART1_RX_STA = 1;
		if(Res == 0x0a){
			if(USART1_RX_STA == 1){
				i = 0;
			    uart_driver.uart1_call_back(USART1_RX_BUF,usart1_len);
				USART1_RX_STA = 0;
			    usart1_len = 0;
			}
			  
		}

  } 
} 

void USART3_IRQHandler(void)
{
		u8 Res;
  static u16 i = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART3);
		
		if(i < USART_REC_LEN ){
			USART3_RX_BUF[i++] = Res;
		  usart3_len++; 
		}
		else{
			i = 0;
			usart1_len = 0;
		}
		if(Res == 0x0d)
			USART3_RX_STA = 1;
		if(Res == 0x0a){
			if(USART3_RX_STA == 1){
				
				uart_driver.uart3_call_back(USART3_RX_BUF,usart3_len);
				USART3_RX_STA = 0;
			  usart3_len = 0;
				i = 0;
			}
				
		}
	
  } 

}

/*blow is about printf function*/

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	
#if UART3_DEBUG
	//USART3->SR;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
    USART_SendData(USART3,(uint8_t)ch);   
	return ch;
#else
		//USART1->SR;
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;	
#endif
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
