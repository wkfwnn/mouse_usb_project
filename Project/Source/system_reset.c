#include "system_reset.h"
#include "stm32f10x_iwdg.h"
#include "wether_first_start.h"

/*about after 1s,system reset*/
static void IWDG_Init(u8 prer,u16 rlr)
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  
	
	IWDG_SetPrescaler(prer);  
	IWDG_SetReload(rlr);  
	
	IWDG_ReloadCounter();  
	IWDG_Enable();  
}

void system_reset( unsigned char data)
{	
	switch(data){
		case NUM_SPEAKER:
			set_bkp_data(USB_HID_MARK);
			break;
		case NUM_HID:
			set_bkp_data(USB_SPEAKER_MARK);
			break;
	}
	/*about 1s*/
	IWDG_Init(4,625);
}

