/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Descriptors for Audio Speaker Demo
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
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* USB Standard Device Descriptor */
const uint8_t Speaker_DeviceDescriptor[] =
{
    SPEAKER_SIZ_DEVICE_DESC,              /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x00,          /* 2.00 */             /* bcdUSB */
    0x02,
    0x00,                                 /* bDeviceClass */
    0x00,                                 /* bDeviceSubClass */
    0x00,                                 /* bDeviceProtocol */
    0x40,                                 /* bMaxPacketSize 40 */
    0x83,                                 /* idVendor */
    0x04,
    0x30,                                 /* idProduct  = 0x5730*/
    0x57,
    0x00,                                 /* bcdDevice */
    0x02,
    1,                                    /* iManufacturer */
    2,                                    /* iProduct */
    3,                                    /* iSerialNumber */
    0x02                                  /* bNumConfigurations */
};

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Speaker_ConfigDescriptor[] =
{
    /* Configuration 1 */
    0x09,                                 /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType */
    0x6D,                                 /* wTotalLength  109 bytes*/
    0x00,
    0x02,                                 /* bNumInterfaces */
    0x01,                                 /* bConfigurationValue */
    0x00,                                 /* iConfiguration */
    0xC0,                                 /* bmAttributes  BUS Powred*/		// 0xC0 -> 0x80  sts change
    0x32,                                 /* bMaxPower = 100 mA*/
    /* 09 byte*/

	// ↓ 端点0 控制端点
    /* USB Speaker Standard interface descriptor */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,      /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x00,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* 09 byte*/

	// ↓ Audio 
    /* USB Speaker Class-specific AC Interface Descriptor */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
    0x00,          /* 1.00 */             /* bcdADC */
    0x01,
    0x27,                                 /* wTotalLength = 39*/
    0x00,
    0x01,                                 /* bInCollection */
    0x01,                                 /* baInterfaceNr */
    /* 09 byte*/

	// 输入
    /* USB Speaker Input Terminal Descriptor */
    AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
    0x01,                                 /* bTerminalID */
    0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
    0x01,
    0x00,                                 /* bAssocTerminal */
    0x01,                                 /* bNrChannels */					   // 声道控制
    0x00,                                 /* wChannelConfig 0x0000  Mono */
    0x00,
    0x00,                                 /* iChannelNames */
    0x00,                                 /* iTerminal */
    /* 12 byte*/

    /* USB Speaker Audio Feature Unit Descriptor */
    0x09,                                 /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
    0x02,                                 /* bUnitID */
    0x01,                                 /* bSourceID */
    0x01,                                 /* bControlSize */
    AUDIO_CONTROL_MUTE,                   /* bmaControls(0) */
    0x00,                                 /* bmaControls(1) */
    0x00,                                 /* iTerminal */
    /* 09 byte*/

    /*USB Speaker Output Terminal Descriptor */
    0x09,      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
    0x03,                                 /* bTerminalID */
    0x01,                                 /* wTerminalType  0x0301*/
    0x03,
    0x00,                                 /* bAssocTerminal */
    0x02,                                 /* bSourceID */
    0x00,                                 /* iTerminal */
    /* 09 byte*/

    /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
    /* Interface 1, Alternate Setting 0                                             */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,       /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
    /* Interface 1, Alternate Setting 1                                           */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,      /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x01,                                 /* bAlternateSetting */
    0x01,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x00,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Audio Streaming Interface Descriptor */
    AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
    0x01,                                 /* bTerminalLink */
    0x01,                                 /* bDelay */
    0x02,                                 /* wFormatTag AUDIO_FORMAT_PCM8  0x0002*/		// <----- PCM格式描述
    0x00,
    /* 07 byte*/

    /* USB Speaker Audio Type I Format Interface Descriptor */
    0x0B,                                 /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
    0x02,                                 /* bNrChannels */               // mod 0x02-->0x01 2 channels   sts need change
    0x01,                                 /* bSubFrameSize */            //  mod 0x02-->0x01 -->02
    0x08,                                 /* bBitResolution */					// <------ 8bit Mode  
    0x01,                                 /* bSamFreqType */
    0xC0,                                 /* tSamFreq 48000 = 0xBB80 */			// <------ 48kHz Freq  mod BB80-->5DC0 24kHz
    0x5D,
    0x00,
    /* 11 byte*/

    /* Endpoint 1 - Standard Descriptor */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    0x01,                                 /* bEndpointAddress 1 out endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */			/* sts need change USB_ENDPOINT_TYPE_ISOCHRONOUS*/
    0x30,                                 /* wMaxPacketSize 192 bytes*/			// <------ MaxPacketSize 192  mod 0xC0-->0x60 96bit --> 0x30
    0x00,
    0x01,                                 /* bInterval */
    0x00,                                 /* bRefresh */
    0x00,                                 /* bSynchAddress */
    /* 09 byte*/
	
    /* Endpoint - Audio Streaming Descriptor*/
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
    0x00,                                 /* bmAttributes */
    0x00,                                 /* bLockDelayUnits */
    0x00,                                 /* wLockDelay */
    0x00,
    /* 07 byte*/
};

/* USB String Descriptor (optional) */
const uint8_t Speaker_StringLangID[SPEAKER_SIZ_STRING_LANGID] =
{
    0x04,
    0x03,
    0x09,
    0x04
}
; /* LangID = 0x0409: U.S. English */

const uint8_t Speaker_StringVendor[SPEAKER_SIZ_STRING_VENDOR] =
{
    SPEAKER_SIZ_STRING_VENDOR, /* Size of manufacturer string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'T', 0, 'e', 0, 'x', 0, 'a', 0, 's', 0, ' ', 0, 'I', 0, 'n', 0,
    's', 0, 't', 0, 'r', 0, 'u', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0,
    's', 0, ' ', 0
};
 
const uint8_t Speaker_StringProduct[SPEAKER_SIZ_STRING_PRODUCT] =
{
    SPEAKER_SIZ_STRING_PRODUCT,  /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'A', 0, 'u', 0, 'd', 0, 'i', 0, 'o', 0, ' ', 0,
    'S', 0, 'p', 0, 'e', 0, 'a', 0, 'k', 0, 'e', 0, 'r', 0
};

uint8_t Speaker_StringSerial[SPEAKER_SIZ_STRING_SERIAL] =
{
    SPEAKER_SIZ_STRING_SERIAL,  /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'P', 0, 'C', 0, 'M', 0, '2', 0, '9', 0, '0', 0, '2', 0
};


/*HID  device*/

/* USB Standard Device Descriptor */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x08,                       /*bMaxPacketSize40*/
    0x86,                       /*idVendor (0x0483)*/
    0x04,
    0x50,                       /*idProduct = 0x5750*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: Configuration value */
    0x00,         /* iConfiguration: Index of string descriptor describing
                                 the configuration*/
    0xC0,         /* bmAttributes: Self powered */
    0x32,         /* MaxPower 100 mA: this current is used for detecting Vbus */

    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x02,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */

    0x81,          /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    0x08,          /* wMaxPacketSize: 2 Bytes max */
    0x00,
    0x02,          /* bInterval: Polling Interval (32 ms) */
    /* 34 */
    	
    0x07,	/* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
    0x01,	/* bEndpointAddress: */
			/*	Endpoint Address (OUT) */
    0x03,	/* bmAttributes: Interrupt endpoint */
    0x01,	/* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x02,	/* bInterval: Polling Interval (20 ms) */
    /* 41 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {                    
    0x05, 0x01,      /* USAGE_PAGE (Vendor Page: 0xFF00) */                       
    0x09, 0x06,            /* USAGE (Demo Kit)               */    
    0xa1, 0x01,            /* COLLECTION (Application)       */            
    /* 6 */
    
    /* Led 1 */        
    0x05, 0x07,            /*     REPORT_ID (1)		     */
    0x19, 0xe0,            /*     USAGE (LED 1)	             */
    0x29, 0xe7,            /*     LOGICAL_MINIMUM (0)        */          
    0x15, 0x00,            /*     LOGICAL_MAXIMUM (1)        */           
    0x25, 0x01,            /*     REPORT_SIZE (8)            */        
    0x95, 0x08,            /*     REPORT_COUNT (1)           */       
    0x75, 0x01,             /*    FEATURE (Data,Var,Abs,Vol) */     

    0x81, 0x02,            /*     REPORT_ID (1)              */
    0x95, 0x01,            /*     USAGE (LED 1)              */
    0x75, 0x08,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 26 */
    
    /* Led 2 */
    0x81, 0x03,            /*     REPORT_ID 2		     */
    0x95, 0x06,            /*     USAGE (LED 2)	             */
    0x75, 0x08,            /*     LOGICAL_MINIMUM (0)        */          
    0x25, 0xFF,            /*     LOGICAL_MAXIMUM (1)        */           
    0x19, 0x00,            /*     REPORT_SIZE (8)            */        
    0x29, 0x65,            /*     REPORT_COUNT (1)           */       
    0x81, 0x80,             /*    FEATURE (Data,Var,Abs,Vol) */     

    0x25, 0x01,            /*     REPORT_ID (2)              */
    0x95, 0x02,            /*     USAGE (LED 2)              */
    0x75, 0x01,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 46 */
    
    /* Led 3 */        
    0x05, 0x08,            /*     REPORT_ID (3)		     */
    0x19, 0x01,            /*     USAGE (LED 3)	             */
    0x29, 0x02,            /*     LOGICAL_MINIMUM (0)        */          
    0x91, 0x02,            /*     LOGICAL_MAXIMUM (1)        */           
    0x95, 0x01,            /*     REPORT_SIZE (8)            */        
    0x75, 0x06,            /*     REPORT_COUNT (1)           */       
    0x91, 0x03,             /*    FEATURE (Data,Var,Abs,Vol) */     

 
    0xc0 	          /*     END_COLLECTION	             */
  }; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'K', 0, 'e', 0, 'y', 0, 'b', 0, 'o', 0, 'a', 0, 'r', 0,
    'd', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
    CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0,'3', 0,'2', 0
  };

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
