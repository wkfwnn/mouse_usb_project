#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			50  	//定义最大接收字节数 50

#define EN_USART1_RX 			  1		//使能（1）/禁止（0）串口1接收
#define EN_USART3_RX        1   //使能（1）/禁止（0）串口3接收
	  	
extern u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 USART1_RX_STA;         		//接收状态标记	

extern u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART3_RX_STA;         		//接收状态标记	

extern u8 usart1_len;
extern u8 usart3_len;

//default debug port is uart1，change to 1,debug port change to uart3
#define UART3_DEBUG     0

typedef struct uart_port
{
	void (*uart1_call_back)(u8 *buff, u8 len);
	void (*uart3_call_back)(u8 *buff,u8 len);

}uart_t;
/*
	@breif:defalut init uart3 and uart1
	@bound:baudrate,can be 9600bps,115200,and other common baudrate
*/
void uart_init(u32 bound);
void USART3_sendData(unsigned char * buff,int len);


#endif


