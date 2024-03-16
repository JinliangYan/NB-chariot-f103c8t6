#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include "stm32f10x.h"

#define BLT_USART_BAUD_RATE 9600

#define UART_BUFF_SIZE      1024

#define PACKAGE_START_FLAG  '$'
#define PACKAGE_END_FLAG    '*'
#define PACKAGE_AT_FLAG     '+'

typedef enum {
    MESSAGE_NONE,
    MESSAGE_MODE_SWITCH,
    MESSAGE_LEFT_JOYSTICK,
    MESSAGE_RIGHT_JOYSTICK,
    MESSAGE_WEAPON_JOYSTICK,
    MESSAGE_WEAPON_ATTACK,
    MESSAGE_TEXT,
    MESSAGE_AT_COMMAND
} receiving_message_type_t;

typedef struct {
    volatile uint16_t datanum;
    uint8_t uart_buff[UART_BUFF_SIZE];
    uint8_t receive_data_flag;
    receiving_message_type_t message_type;
} bt_received_data_t;

void Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen);
void Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str);

char* get_rebuff(uint16_t* len);
void clean_rebuff(void);

#endif /* __USART2_H */
