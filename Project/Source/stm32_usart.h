//
// ע��:�ڷ���û����ɵ�������ٴε����κ�һ�����ͺ����ᵼ���̶߳���
// ע��:����Ĭ�Ͽ������ڽ����ж�
#ifndef _stm32_usart_h_
#define _stm32_usart_h_
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

// ��������
void USART1_Configuration(void);
// �жϷ���
void USART1_SendString(u8 *buffer,u8 length);
// ���ַ����жϷ��� P.S.�������ǰ�ᵼ���̶߳���
void USART1_SendData(u8 buffer);
// �жϷ�����
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

// ���º������ڴ������ú��������
// ʱ������
void USART1_RCC_Configuration(void);
// ������������
void USART1_GPIO_Configuration(void);
// �ж�����
void USART1_NVIC_Configuration(void);
#endif
