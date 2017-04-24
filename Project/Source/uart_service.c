#include "uart_service.h"

void uart1_irq_service(u8 *buff ,u8 len)
{
	u8 i = 0;
	printf("uart1bufflen:%d\r\n",len);
	for(i = 0;i < len;i++){
		printf("%c",buff[i]);
	}
}

void uart3_irq_service(u8 *buff ,u8 len)
{
	u8 i = 0;
	printf("uart3bufflen:%d\r\n",len);
	for(i = 0;i < len;i++){
		printf("%c",buff[i]);
	}
}
