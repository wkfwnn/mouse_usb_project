#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			50  	//�����������ֽ��� 50

#define EN_USART1_RX 			  1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART3_RX        1   //ʹ�ܣ�1��/��ֹ��0������3����
	  	
extern u8  USART1_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART1_RX_STA;         		//����״̬���	

extern u8  USART3_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART3_RX_STA;         		//����״̬���	

extern u8 usart1_len;
extern u8 usart3_len;

//default debug port is uart1��change to 1,debug port change to uart3
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


