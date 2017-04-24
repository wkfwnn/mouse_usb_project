/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Audio Speaker Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_prop.h"

//#include "usb_lib.h"
#include "usbio.h"

#include "stm32_usart.h"
#include "stm32_dma.h"
#include "usart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern uint32_t MUTE_DATA;
extern uint16_t In_Data_Offset;
extern uint16_t Out_Data_Offset;
extern uint8_t Stream_Buff[];
extern uint8_t IT_Clock_Sent;

extern u8 TimeCount;
extern u32 mData_Len;

#define	REPORT_COUNT							48//�˵㳤��
#define ENDP2_TXADDR        (0x118)
//uint8_t USB_Receive_Buffer[REPORT_COUNT];
extern uint8_t Stream_Buff[48];
uint8_t USB_Send_Buffer[REPORT_COUNT];
extern uint8_t USB_Received_Flag;
uint8_t g_dataSerialNumber;

extern uint8_t USB_Received_Flag;
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

  
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ͨ��USB��������
  * @param  data ���ݴ洢�׵�ַ
  * @param  dataNum ���͵������ֽ���
  * @retval ���͵��ֽ���
  */
uint32_t USB_SendData(uint8_t *data,uint32_t dataNum)
{
	#ifndef STM32F10X_CL
	//������ͨ��USB���ͳ�ȥ
	UserToPMABufferCopy(data, ENDP2_TXADDR, dataNum);
	SetEPTxCount(ENDP2, REPORT_COUNT);
	SetEPTxValid(ENDP2);
	#else
	USB_SIL_Write(EP2_IN, data, dataNum);
	#endif
	return dataNum;  
}
/**
  * @brief  ���մ�USB��ȡ������
  * @param  data ���ݴ洢�׵�ַ
  * @param  dataNum ׼����ȡ�������ֽ���
  * @retval ��ȡ���ֽ���
  */
uint32_t USB_GetData(uint8_t *data,uint32_t dataNum)
{
    uint32_t len=0;
	if(dataNum>sizeof(Stream_Buff)){
		dataNum = sizeof(Stream_Buff);
	}
	for(len=0;len<dataNum;len++){
		*data=Stream_Buff[len];
		data++;
	}
    return dataNum;
}

u8  Flag_Uart_Send = 0;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	uint8_t data[128];
	u8 aa[10];
	uint32_t i=0,ret=0;
	
    Set_System();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(256000 );
	DMA_Configuration();
	Set_USBClock();
	USB_Config();
	USB_Init();
	Speaker_Config();
   // printf("helloworld\n");
     while(1) {
	 	if(Flag_Uart_Send != 1){
		 	//DMA_SetCurrDataCounter(DMA1_Channel4, 48);
		 	//DMA_Cmd(DMA1_Channel4,ENABLE);
			Flag_Uart_Send = 1;
	 	}
        
		if(USB_Received_Flag){
			USB_Received_Flag = FALSE;
			//printf("receive data\n");
		}

     }
}

void USB_SendString(u8 *str)
{
     //u8 ii=0;   
     while(*str)
     {
         //Transi_Buffer[ii++]=*(str++);
         //if (ii == nReportCnt) break;
     }
//     UserToPMABufferCopy(Transi_Buffer, ENDP2_TXADDR, nReportCnt);
//     SetEPTxValid(ENDP2);
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
        {}
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/