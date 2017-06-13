/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : All processing related to Audio Speaker Demo
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
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "wether_first_start.h"

#define NUM_REQUESTS           2

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t MUTE_DATA = 0;


DEVICE Device_Table =
{
    EP_NUM,
    1
};

DEVICE_PROP Device_Property_Speaker =
{
    Speaker_init,
    Speaker_Reset,
    Speaker_Status_In,
    Speaker_Status_Out,
    Speaker_Data_Setup,
    Speaker_NoData_Setup,
    Speaker_Get_Interface_Setting,
    Speaker_GetDeviceDescriptor,
    Speaker_GetConfigDescriptor,
    Speaker_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
};

USER_STANDARD_REQUESTS User_Standard_Request_Speaker =
{
    Speaker_GetConfiguration,
    Speaker_SetConfiguration,
    Speaker_GetInterface,
    Speaker_SetInterface,
    Speaker_GetStatus,
    Speaker_ClearFeature,
    Speaker_SetEndPointFeature,
    Speaker_SetDeviceFeature,
    Speaker_SetDeviceAddress
};


ONE_DESCRIPTOR Device_Descriptor_Speaker =
{
    (uint8_t*)Speaker_DeviceDescriptor,
    SPEAKER_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor_Speaker =
{
    (uint8_t*)Speaker_ConfigDescriptor,
    SPEAKER_SIZ_CONFIG_DESC
};

ONE_DESCRIPTOR String_Descriptor_Speaker[4] =
{
    {(uint8_t*)Speaker_StringLangID, SPEAKER_SIZ_STRING_LANGID},
    {(uint8_t*)Speaker_StringVendor, SPEAKER_SIZ_STRING_VENDOR},
    {(uint8_t*)Speaker_StringProduct, SPEAKER_SIZ_STRING_PRODUCT},
    {(uint8_t*)Speaker_StringSerial, SPEAKER_SIZ_STRING_SERIAL},
};

/* Extern variables ----------------------------------------------------------*/
extern uint16_t In_Data_Offset;
extern uint16_t Out_Data_Offset;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Speaker_init.
* Description    : Speaker init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_init()
{
    /* Update the serial number string descriptor with the data from the unique
    ID*/
    Get_SerialNum();

    /* Initialize the current configuration */
    pInformation->Current_Configuration = 0;

    /* Connect the device */
    PowerOn();

    /* Perform basic device initialization operations */
    USB_SIL_Init();

    bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : Speaker_Reset.
* Description    : Speaker reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_Reset()
{
    /* Set Speaker device as not configured state */
    pInformation->Current_Configuration = 0;

    /* Current Feature initialization */
    pInformation->Current_Feature = Speaker_ConfigDescriptor[7];

    SetBTABLE(BTABLE_ADDRESS);

    /* Initialize Endpoint 0 */
    SetEPType(ENDP0, EP_CONTROL);
    SetEPTxStatus(ENDP0, EP_TX_NAK);
    SetEPRxAddr(ENDP0, ENDP0_RXADDR);
    SetEPRxCount(ENDP0, Device_Property[CURRENT_USB_DEVICE_ENUM]->MaxPacketSize);
    SetEPTxAddr(ENDP0, ENDP0_TXADDR);
    Clear_Status_Out(ENDP0);
    SetEPRxValid(ENDP0);

    /* Initialize Endpoint 1 */
    SetEPType(ENDP1, EP_ISOCHRONOUS);
    SetEPDblBuffAddr(ENDP1, ENDP1_BUF0Addr, ENDP1_BUF1Addr);
    SetEPDblBuffCount(ENDP1, EP_DBUF_OUT, 0xc0);
    ClearDTOG_RX(ENDP1);
    ClearDTOG_TX(ENDP1);
    ToggleDTOG_TX(ENDP1);
    SetEPRxStatus(ENDP1, EP_RX_VALID);
    SetEPTxStatus(ENDP1, EP_TX_DIS);

    SetEPRxValid(ENDP0);
    /* Set this device to response on default address */
    SetDeviceAddress(0);

    bDeviceState = ATTACHED;

    In_Data_Offset = 0;
    Out_Data_Offset = 0;
}
/*******************************************************************************
* Function Name  : Speaker_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_SetConfiguration(void)
{
    DEVICE_INFO *pInfo = &Device_Info;

    if (pInfo->Current_Configuration != 0)
    {
        /* Device configured */
        bDeviceState = CONFIGURED;
    }
}



void Speaker_GetConfiguration(void)
{
		


}
/*******************************************************************************
* Function Name  : Speaker_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : Speaker_Status_In.
* Description    : Speaker Status In routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_Status_In(void)
{}

/*******************************************************************************
* Function Name  : Speaker_Status_Out.
* Description    : Speaker Status Out routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_Status_Out (void)
{}

/*******************************************************************************
* Function Name  : Speaker_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : None.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Speaker_Data_Setup(uint8_t RequestNo)
{
    uint8_t *(*CopyRoutine)(uint16_t);
    CopyRoutine = NULL;

    if ((RequestNo == GET_CUR) || (RequestNo == SET_CUR))
    {
        CopyRoutine = Mute_Command;
    }

    else
    {
        return USB_UNSUPPORT;
    }

    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);
    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Speaker_NoData_Setup
* Description    : Handle the no data class specific requests.
* Input          : None.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Speaker_NoData_Setup(uint8_t RequestNo)
{
    return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Speaker_GetDeviceDescriptor.
* Description    : Get the device descriptor.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *Speaker_GetDeviceDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Device_Descriptor_Speaker);
}

/*******************************************************************************
* Function Name  : Speaker_GetConfigDescriptor.
* Description    : Get the configuration descriptor.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Speaker_GetConfigDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Config_Descriptor_Speaker);
}

/*******************************************************************************
* Function Name  : Speaker_GetStringDescriptor.
* Description    : Get the string descriptors according to the needed index.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Speaker_GetStringDescriptor(uint16_t Length)
{
    uint8_t wValue0 = pInformation->USBwValue0;

    if (wValue0 > 4)
    {
        return NULL;
    }
    else
    {
        return Standard_GetDescriptorData(Length, &String_Descriptor_Speaker[wValue0]);
    }
}

/*******************************************************************************
* Function Name  : Speaker_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.
* Input1         : uint8_t: Interface : interface number.
* Input2         : uint8_t: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Speaker_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
    if (AlternateSetting > 1)
    {
        return USB_UNSUPPORT;
    }
    else if (Interface > 1)
    {
        return USB_UNSUPPORT;
    }
    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Mute_Command
* Description    : Handle the GET MUTE and SET MUTE command.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Mute_Command(uint16_t Length)
{

    if (Length == 0)
    {
        pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w;
        return NULL;
    }
    else
    {
        return((uint8_t*)(&MUTE_DATA));
    }
}


/*about hid key board*/

uint32_t ProtocolValue;
uint8_t Report_Buf[2];   
__IO uint8_t Request = 0;

DEVICE_PROP Device_Property_Hid =
  {
    CustomHID_init,
    CustomHID_Reset,
    CustomHID_Status_In,
    CustomHID_Status_Out,
    CustomHID_Data_Setup,
    CustomHID_NoData_Setup,
    CustomHID_Get_Interface_Setting,
    CustomHID_GetDeviceDescriptor,
    CustomHID_GetConfigDescriptor,
    CustomHID_GetStringDescriptor,
    0,
    0x08 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests_Hid =
  {
    CustomHID_GetConfiguration,
    CustomHID_SetConfiguration,
    CustomHID_GetInterface,
    CustomHID_SetInterface,
    CustomHID_GetStatus,
    CustomHID_ClearFeature,
    CustomHID_SetEndPointFeature,
    CustomHID_SetDeviceFeature,
    CustomHID_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor_Hid =
  {
    (uint8_t*)CustomHID_DeviceDescriptor,
    CUSTOMHID_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor_Hid =
  {
    (uint8_t*)CustomHID_ConfigDescriptor,
    CUSTOMHID_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR CustomHID_Report_Descriptor =
  {
    (uint8_t *)CustomHID_ReportDescriptor,
    CUSTOMHID_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR CustomHID_Hid_Descriptor =
  {
    (uint8_t*)CustomHID_ConfigDescriptor + CUSTOMHID_OFF_HID_DESC,
    CUSTOMHID_SIZ_HID_DESC
  };

ONE_DESCRIPTOR String_Descriptor_Hid[4] =
  {
    {(uint8_t*)CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID},
    {(uint8_t*)CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR},
    {(uint8_t*)CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT},
    {(uint8_t*)CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*CustomHID_SetReport_Feature function prototypes*/
uint8_t *CustomHID_SetReport_Feature(uint16_t Length);

/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CustomHID_init.
* Description    : Custom HID init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_init(void)
{
  /* Update the serial number string descriptor with the data from the unique 
  ID*/
  Get_SerialNum();
    
  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : CustomHID_Reset.
* Description    : Custom HID reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Reset(void)
{
  /* Set CustomHID_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/
  
  /* Current Feature initialization */
  pInformation->Current_Feature = CustomHID_ConfigDescriptor[7];
 
  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property[CURRENT_USB_DEVICE_ENUM]->MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPRxAddr(ENDP1, ENDP1_RXADDR);
  SetEPTxCount(ENDP1, 8);
  SetEPRxCount(ENDP1, 1);
  SetEPRxStatus(ENDP1, EP_RX_VALID);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  bDeviceState = ATTACHED;
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Update the device state to configured and command the ADC 
*                  conversion.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetConfiguration(void)
{
  if (pInformation->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
    
    /* Start ADC Software Conversion */ 
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)|| defined(STM32F37X)
    ADC_SoftwareStartConv(ADC1);
#elif defined (STM32F30X)
    ADC_StartConversion(ADC1);
#else
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
#endif /* STM32L1XX_XD */
  }
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : CustomHID_Status_In.
* Description    : Joystick status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_In(void)
{  
  BitAction Led_State;
  
  if (Report_Buf[1] == 0)
  {
    Led_State = Bit_RESET;
  }
  else 
  {
    Led_State = Bit_SET;
  }
  
  switch (Report_Buf[0])  
  {
    /*Change LED's status according to the host report*/
    
  case 1: /* Led 1 */ 
    if (Led_State != Bit_RESET)
    {
      //STM_EVAL_LEDOn(LED1);
    }
    else
    {
    	//STM_EVAL_LEDOff(LED1);
    }
    break;
  case 2:   /* Led 2 */    
    if (Led_State != Bit_RESET)
    {
      //STM_EVAL_LEDOn(LED2);
    }
    else
    {
      //STM_EVAL_LEDOff(LED2);
    }
    break;
  case 3:/* Led 3 */    
    if (Led_State != Bit_RESET)
    {
      //STM_EVAL_LEDOn(LED3);
    }
    else
    {
      //STM_EVAL_LEDOff(LED3);
    }
    break;
  case 4:/* Led 4 */    
    if (Led_State != Bit_RESET)
	{
		//STM_EVAL_LEDOn(LED4);
    }
    else
    {
      //STM_EVAL_LEDOff(LED4);
    }
    break;
  default:
    //STM_EVAL_LEDOff(LED1);
   // STM_EVAL_LEDOff(LED2);
    //STM_EVAL_LEDOff(LED3);
    //STM_EVAL_LEDOff(LED4); 
    break;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_Status_Out
* Description    : Joystick status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CustomHID_Status_Out (void)
{
}

/*******************************************************************************
* Function Name  : CustomHID_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);
  
  if (pInformation->USBwIndex != 0) 
    return USB_UNSUPPORT;    
  
  CopyRoutine = NULL;
  
  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
        )
  {
    
    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = CustomHID_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = CustomHID_GetHIDDescriptor;
    }
    
  } /* End of GET_DESCRIPTOR */
  
  /*** GET_PROTOCOL, GET_REPORT, SET_REPORT ***/
  else if ( (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) )
  {         
    switch( RequestNo )
    {
    case GET_PROTOCOL:
      CopyRoutine = CustomHID_GetProtocolValue;
      break;
    case SET_REPORT:
      CopyRoutine = CustomHID_SetReport_Feature;
      Request = SET_REPORT;
      break;
    default:
      break;
    }
  }
  
  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }
  
  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetReport_Feature
* Description    : Set Feature request handling
* Input          : Length.
* Output         : None.
* Return         : Buffer
*******************************************************************************/
uint8_t *CustomHID_SetReport_Feature(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 2;
    return NULL;
  }
  else
  {
    return Report_Buf;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT CustomHID_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
  {
    return CustomHID_SetProtocol();
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : CustomHID_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor_Hid);
}

/*******************************************************************************
* Function Name  : CustomHID_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor_Hid);
}

/*******************************************************************************
* Function Name  : CustomHID_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *CustomHID_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else 
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor_Hid[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : CustomHID_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &CustomHID_Report_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &CustomHID_Hid_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT CustomHID_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t *CustomHID_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

USER_STANDARD_REQUESTS *User_Standard_Requests[NUM_REQUESTS] = {
	&User_Standard_Request_Speaker,
	&User_Standard_Requests_Hid
};

DEVICE_PROP *Device_Property[NUM_REQUESTS] = {
	&Device_Property_Speaker,
	&Device_Property_Hid
};


