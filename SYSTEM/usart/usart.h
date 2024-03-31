#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

/**
 * \brief   蓝牙串口接收缓冲区大小
 */
#define UART_BUFF_SIZE      1024

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
    volatile uint16_t datanum;             /*!< 数据字节数 */
    uint8_t uart_buff[UART_BUFF_SIZE];     /*!< 数据缓冲区 */
    uint8_t receive_data_flag;             /*!< 接收完成标志位 */
    receiving_message_type_t message_type; /*!< 消息类型 */
} bt_received_data_t;

/**
 * \brief   存储武器接收数据
 */
typedef struct {
    volatile uint16_t datanum;         /*!< 数据字节数 */
    uint8_t uart_buff[UART_BUFF_SIZE]; /*!< 数据缓冲区 */
    uint8_t receive_data_flag;         /*!< 接收完成标志位 */
} weapon_received_data_t;

extern bt_received_data_t bt_received_data;
extern weapon_received_data_t weapon_received_data;


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
