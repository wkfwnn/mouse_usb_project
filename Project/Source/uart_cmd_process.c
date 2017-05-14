#include "uart_cmd_process.h"
#include "uart_cmd_resolve.h"
#include "stdio.h"
void uart_cmd_process(void)
{
	if(uart_cmd & INJECT){
		printf(" process inject\n");
		uart_cmd &=~INJECT;
	}else if(uart_cmd & AUDIO){
		printf("process AUDIO\n");
		uart_cmd &=~AUDIO;
	}else if(uart_cmd & KEYBOARD){
		printf("process KEYBOARD\n");
		uart_cmd &=~KEYBOARD;
	}else if(uart_cmd & EXEC){
		printf(" process EXEC\n");
		uart_cmd &=~EXEC;
	}else {
		goto exit_uart_cmd_process;
	}	


exit_uart_cmd_process:
	return;



}


