#include "uart_cmd_resolve.h"
#include "string.h"

volatile unsigned char uart_cmd = 0x00;

void uart_cmd_resolve(const unsigned char *in,int len)
{	//(void *)len;
	if(strncmp(in,"inject",6)  == 0){
		uart_cmd |= INJECT;
	}else if(strncmp(in,"audio",5)  == 0){
		uart_cmd |= AUDIO;
	}else if(strncmp(in,"exec",4)  == 0){
		uart_cmd |= EXEC;
	}else if(strncmp(in,"keyboard",4)  == 0){
		uart_cmd |= KEYBOARD;
	}else{
		goto exit_uart_cmd_resolve;
	}

exit_uart_cmd_resolve:
	return;
}

