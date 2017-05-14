/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define MAX_STREAM_BUF_SIZE     20

uint8_t Stream_Buff[15*1024];	// 192 = 48 * 2 * 2 + 2(头两个字节为起始标志) mod 24*1*2+2 -> 16*2*2+2 =50
volatile uint8_t current_stream = 0;

uint8_t test_buff[48] = {0x00,};
uint16_t In_Data_Offset;

u32 mData_Len = 0;

volatile uint8_t USB_Received_Flag = FALSE;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback
* Description    : Endpoint 1 out callback routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
    uint16_t Data_Len;       /* data length*/

	static int i = 0;	
		
    if (GetENDPOINT(ENDP1) & EP_DTOG_TX)
    {
    	
        /*read from ENDP1_BUF0Addr buffer*/
		Data_Len = GetEPDblBuf0Count(ENDP1);
		mData_Len += Data_Len;
		//printf("%d\n",Data_Len);
		PMAToUserBufferCopy(&Stream_Buff[mData_Len], ENDP1_BUF0Addr, Data_Len);
		//USB_Received_Flag = 1;
		PA8 = 1;
		PB15 = 1;
		if(i == 0){
			DMA_Cmd(DMA1_Channel4,DISABLE);
			//DMA1_Channel4->CMAR = DataNumber;  
			DMA_SetCurrDataCounter(DMA1_Channel4, Data_Len);
			DMA_Cmd(DMA1_Channel4,ENABLE);
		}
		i = 1;
    }
    else
    {
		PA8 = 0;
		PB15 = 1;
        /*read from ENDP1_BUF1Addr buffer*/
		Data_Len = GetEPDblBuf1Count(ENDP1);
		mData_Len += Data_Len;
		//printf("%d\n",Data_Len);
		PMAToUserBufferCopy(&Stream_Buff[mData_Len], ENDP1_BUF1Addr, Data_Len);
		//USB_Received_Flag = 1;
		//DMA_Cmd(DMA1_Channel4,DISABLE);
		//DMA_SetCurrDataCounter(DMA1_Channel4, Data_Len);
		//DMA_Cmd(DMA1_Channel4,ENABLE);
    }
    FreeUserBuffer(ENDP1, EP_DBUF_OUT);
    In_Data_Offset += Data_Len;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

