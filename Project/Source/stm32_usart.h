//
// 注意:在发送没有完成的情况下再次调用任何一个发送函数会导致线程堵塞
// 注意:程序默认开启串口接收中断
#ifndef _stm32_usart_h_
#define _stm32_usart_h_
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

// 串口配置
void USART1_Configuration(void);
// 中断发送
void USART1_SendString(u8 *buffer,u8 length);
// 单字符非中断发送 P.S.发送完成前会导致线程堵塞
void USART1_SendData(u8 buffer);
// 中断服务函数
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

// 以下函数已在串口配置函数中添加
// 时钟配置
void USART1_RCC_Configuration(void);
// 串口引脚配置
void USART1_GPIO_Configuration(void);
// 中断配置
void USART1_NVIC_Configuration(void);
#endif
