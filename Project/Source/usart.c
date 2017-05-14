#include "sys.h"
#include "usart.h"	  
#include "uart_service.h"

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
u8 USART1_RX_STA = 0;       //����״̬���	  
u8 usart1_len = 0;

#endif


#if EN_USART3_RX

u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART3_RX_STA = 0;              //����״̬���
u8 usart3_len = 0;

#endif


static uart_t uart_driver = {

	NULL,
	NULL
};

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //ʹ��USART3��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	USART_DeInit(USART1);  //��λ����1
	USART_DeInit(USART3);  //��λ����3
	
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
 
	//USART3_TX  PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10
 
	
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

	//USART3_RX  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	
#if EN_USART1_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
#endif


#if EN_USART3_RX
	//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�

#endif

    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 1
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 3

	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); 

	/*call back function*/
	uart_driver.uart1_call_back = uart1_irq_service;
	uart_driver.uart3_call_back = uart3_irq_service;

}







//����1�жϷ������
void USART1_IRQHandler(void)                	
	{
	u8 Res;
  static u16 i = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
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
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
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

/*ʹ��microLib�ķ���*/
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
 
