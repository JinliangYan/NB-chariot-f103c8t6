#ifndef USART_H
#define USART_H
#include "stdio.h"
#include "sys.h"

/**
 * \brief   蓝牙串口接收缓冲区大小
 */
#define BUFF_SIZE 256

/**
 * \brief   蓝牙接收消息类型
*/
typedef enum {
    BT_MESSAGE_NONE,            /*!< 默认值 */
    BT_MESSAGE_MODE_SWITCH,     /*!< 模式切换命令 */
    BT_MESSAGE_LEFT_JOYSTICK,   /*!< 左摇杆数据 */
    BT_MESSAGE_TURN,            /*!< 转向数据 */
    BT_MESSAGE_WEAPON_JOYSTICK, /*!< 武器摇杆数据 */
    BT_MESSAGE_WEAPON_ATTACK,   /*!< 武器攻击命令 */
    BT_MESSAGE_WEAPON_SKILL,    /*!< 武器技能释放命令 */
    BT_MESSAGE_AT_COMMAND,      /*!< AT指令回复 */
    BT_MESSAGE_GAME,            /*!< 游戏信息 */
} bt_receiving_message_type_t;

typedef enum { WEAPON_MESSAGE_NONE, WEAPON_MESSAGE_IR_SIG, WEAPON_MESSAGE_FEEDBACK } weapon_receiving_message_type_t;

typedef enum {
    SLAVER_MESSAGE_NONE,
    SLAVER_MESSAGE_VIDEO,
    SLAVER_MESSAGE_MODEL,
    SLAVER_MESSAGE_INFO,
} slaver_board_message_type_t;

/**
 * \brief   存储蓝牙接收数据
 */
typedef struct {
    volatile uint16_t datanum;                /*!< 数据字节数 */
    uint8_t uart_buff[BUFF_SIZE];             /*!< 数据缓冲区 */
    uint8_t receive_data_flag;                /*!< 接收完成标志位 */
    bt_receiving_message_type_t message_type; /*!< 消息类型 */
} bt_received_data_t;

/**
 * \brief   存储武器接收数据
 */
typedef struct {
    volatile uint16_t datanum;                    /*!< 数据字节数 */
    uint8_t uart_buff[BUFF_SIZE];                 /*!< 数据缓冲区 */
    uint8_t receive_data_flag;                    /*!< 接收完成标志位 */
    weapon_receiving_message_type_t message_type; /*!< 消息类型 */
} weapon_received_data_t;

/**
 * \brief   从板接收数据缓冲区
 */
typedef struct {
    volatile uint16_t datanum;                /*!< 数据字节数 */
    uint8_t uart_buff[BUFF_SIZE];             /*!< 数据缓冲区 */
    uint8_t receive_data_flag;                /*!< 接收完成标志位 */
    slaver_board_message_type_t message_type; /*!< 消息类型 */
} slaver_received_data_t;

extern bt_received_data_t bt_received_data;
extern weapon_received_data_t weapon_received_data;
extern slaver_received_data_t slaver_received_data;

void usart_send_byte(USART_TypeDef* USARTx, uint16_t data);

void usart1_init_remap(uint32_t bound);
void usart2_init(uint32_t bound);
void usart3_init(uint32_t bound);

void usart1_send_byte(uint8_t data);
void usart1_send_nbyte(uint8_t* data, uint16_t size);
void usart1_send_str(uint8_t* data);

void usart2_send_byte(uint8_t data);
void usart2_send_nbyte(uint8_t* data, uint16_t size);
void usart2_send_str(uint8_t* data);

void usart3_send_byte(uint8_t data);
void usart3_send_nbyte(uint8_t* data, uint16_t size);
void usart3_send_str(char* data);

#endif
