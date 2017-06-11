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

#include "stm32_dma.h"
#include "usart.h"
#include "uart_cmd_process.h"
#include "state.h"
#include "wether_first_start.h"
#include "system_reset.h"


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

#define	REPORT_COUNT				48//端点长度
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
  * @brief  通过USB发送数据
  * @param  data 数据存储首地址
  * @param  dataNum 发送的数据字节数
  * @retval 发送的字节数
  */
uint32_t USB_SendData(uint8_t *data,uint32_t dataNum)
{
	#ifndef STM32F10X_CL
	//将数据通过USB发送出去
	UserToPMABufferCopy(data, ENDP2_TXADDR, dataNum);
	SetEPTxCount(ENDP2, REPORT_COUNT);
	SetEPTxValid(ENDP2);
	#else
	USB_SIL_Write(EP2_IN, data, dataNum);
	#endif
	return dataNum;  
}

u8  Flag_Uart_Send = 0;


void bsp_init()
{
	uart_init(115200);
	whether_first_start();
    Set_System();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	DMA_Configuration();
	Set_USBClock();
	USB_Config();
	USB_Init();
	//Speaker_Config();
	GPIO_Config();

}
int main(void)
{
	bsp_init();
    while(1) {
	 	uart_cmd_process();
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
