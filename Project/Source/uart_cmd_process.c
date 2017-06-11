#include "uart_cmd_process.h"
#include "uart_cmd_resolve.h"
#include "stdio.h"
#include "usart.h"	 


unsigned char file_name[] = "test_data.txt";
unsigned char test_data[] = "helloworlddsaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\
dsaaaaaaaaaaaaaaaaaaaaaaaaafdsDFfsFFFFFFFFFFFFFFFFFFFF";
 
unsigned char len_buf[4] = {0x00,};
unsigned char len_string[] = "length";


void uart_cmd_process(void)
{
	int filelen;
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
	}else if(uart_cmd & SEND){
		printf(" process SEND\n");
	  filelen = sizeof(test_data);
		len_buf[0] = (filelen & 0xff000000) >> 24;
		len_buf[1] = (filelen & 0x00ff0000) >> 16;
		len_buf[2] = (filelen & 0x0000ff00) >> 8;
		len_buf[3] = (filelen & 0x000000ff);
		USART3_sendData(len_string,6);
		USART3_sendData(len_buf,4);
		USART3_sendData(test_data,sizeof(test_data));
		uart_cmd &=~SEND;
	}else {
		goto exit_uart_cmd_process;
	}	
exit_uart_cmd_process:
	return;
}


