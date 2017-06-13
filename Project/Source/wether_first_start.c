
#include "wether_first_start.h"
#include "stdio.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

unsigned char usb_type = NUM_HID;

void whether_first_start(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE);		

	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
	if( BKP_ReadBackupRegister(BKP_DR1) != USB_HID_MARK){
		printf("enum usb hid device\n");
		BKP_WriteBackupRegister(BKP_DR1,USB_HID_MARK);
		usb_type = NUM_HID;
	}else{
		printf("enum usb speaker\n");
		usb_type = NUM_SPEAKER;
	}
}


void set_bkp_data(unsigned int data)
{
	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
	BKP_WriteBackupRegister(BKP_DR1,data);
}

