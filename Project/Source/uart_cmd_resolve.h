#ifndef __UART_CMD_RESOLVE_H_
#define __UART_CMD_RESOLVE_H_


/* cmd from wifi device or send to wifi device*/
#define INJECT      1 << 0
#define AUDIO       1 << 1
#define KEYBOARD    1 << 2
#define EXEC        1 << 3
#define NAME        1 << 4
#define TYPE        1 << 5
#define LENGTH      1 << 6
#define DATA        1 << 7

extern volatile unsigned char uart_cmd;
void uart_cmd_resolve(const unsigned char * in,int len);

#endif
