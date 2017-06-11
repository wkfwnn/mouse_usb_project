#include "uart_service.h"
#include "uart_cmd_resolve.h"

#define USE_UART1_RESOLVE    0
#define USE_UART3_RESOLVE    1

void uart1_irq_service(u8 *buff ,u8 len)
{
	u8 i = 0;
	printf("uart1bufflen:%d\r\n",len);
	for(i = 0;i < len;i++){
		printf("%c",buff[i]);
	}
#if USE_UART1_RESOLVE
	uart_cmd_resolve(buff,len);
#endif
}

void uart3_irq_service(u8 *buff ,u8 len)
{
	u8 i = 0;
	printf("uart3bufflen:%d\r\n",len);
	for(i = 0;i < len;i++){
		printf("%c",buff[i]);
	}
#if USE_UART3_RESOLVE
	uart_cmd_resolve(buff,len);
#endif
}
