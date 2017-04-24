#ifndef _UART_SERVICE_H_
#define _UART_SERVICE_H_

#include "stdio.h"	
#include "sys.h" 

void uart3_irq_service(u8 *buff ,u8 len);
void uart1_irq_service(u8 *buff ,u8 len);

#endif


