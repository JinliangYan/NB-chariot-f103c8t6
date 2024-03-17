#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include "stm32f10x.h"

/**
 * \brief   蓝牙串口波特率
 */
#define BLT_USART_BAUD_RATE 9600

/**
 * \brief   蓝牙串口接收缓冲区大小
 */
#define UART_BUFF_SIZE      1024

/**
 * \brief   数据包起始位
 */
#define PACKAGE_START_FLAG  '$'

/**
 * \brief   数据包结束位
 */
#define PACKAGE_END_FLAG    '*'

/**
 * \brief   AT指令回复起始位
 */
#define PACKAGE_AT_FLAG     '+'

/**
 * \brief   蓝牙接收消息类型
*/
typedef enum {
    MESSAGE_NONE,            /*!< 默认值 */
    MESSAGE_MODE_SWITCH,     /*!< 模式切换命令 */
    MESSAGE_LEFT_JOYSTICK,   /*!< 左摇杆数据 */
    MESSAGE_RIGHT_JOYSTICK,  /*!< 右摇杆数据 */
    MESSAGE_WEAPON_JOYSTICK, /*!< 武器摇杆数据 */
    MESSAGE_WEAPON_ATTACK,   /*!< 武器攻击命    令 */
    MESSAGE_TEXT,            /*!< 文本数据 */
    MESSAGE_AT_COMMAND       /*!< AT指令回复 */
} receiving_message_type_t;

/**
 * \brief   存储蓝牙接收数据
 */
typedef struct {
    volatile uint16_t datanum;              /*!< 数据字节数 */
    uint8_t uart_buff[UART_BUFF_SIZE];      /*!< 数据缓冲区 */
    uint8_t receive_data_flag;              /*!< 接收完成标志位 */
    receiving_message_type_t message_type;  /*!< 消息类型 */
} bt_received_data_t;

void Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen);
void Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str);

char* get_rebuff(uint16_t* len);
void clean_rebuff(void);

#endif /* __USART2_H */
