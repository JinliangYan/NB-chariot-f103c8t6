#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

//注意:使用蓝牙模块时波特率使用9600,不能超过9600波特率

void usart_send_byte(USART_TypeDef* USARTx, uint16_t data);

void usart1_init_remap(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);

void usart1_send_byte(u8 data);
void usart1_send_nbyte(u8* data, u16 size);
void usart1_send_str(u8* data);

void usart2_send_byte(u8 data);
void usart2_send_nbyte(u8* data, u16 size);
void usart2_send_str(u8* data);

void usart3_send_byte(u8 data);
void usart3_send_nbyte(u8* data, u16 size);
void usart3_send_str(u8* data);

#endif
