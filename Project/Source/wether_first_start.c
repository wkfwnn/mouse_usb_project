
#include "wether_first_start.h"
#include "stdio.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"



unsigned char is_first_start = 1;

void whether_first_start(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR,ENABLE);		

	PWR_BackupAccessCmd(ENABLE);
	BKP_ClearFlag();
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5){
		printf("first time start\n");
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
		is_first_start = 1;
	}else{
		printf("not first time start\n");
		is_first_start = 0;
	}
}

