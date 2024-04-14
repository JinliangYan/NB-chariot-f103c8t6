#ifndef USART_H
#define USART_H
#include "stdio.h"
#include "sys.h"

/**
 * \brief   �������ڽ��ջ�������С
 */
#define BUFF_SIZE 256

/**
 * \brief   ����������Ϣ����
*/
typedef enum {
    BT_MESSAGE_NONE,            /*!< Ĭ��ֵ */
    BT_MESSAGE_MODE_SWITCH,     /*!< ģʽ�л����� */
    BT_MESSAGE_LEFT_JOYSTICK,   /*!< ��ҡ������ */
    BT_MESSAGE_TURN,            /*!< ת������ */
    BT_MESSAGE_WEAPON_JOYSTICK, /*!< ����ҡ������ */
    BT_MESSAGE_WEAPON_ATTACK,   /*!< ������������ */
    BT_MESSAGE_WEAPON_SKILL,    /*!< ���������ͷ����� */
    BT_MESSAGE_AT_COMMAND,      /*!< ATָ��ظ� */
    BT_MESSAGE_GAME,            /*!< ��Ϸ��Ϣ */
} bt_receiving_message_type_t;

typedef enum { WEAPON_MESSAGE_NONE, WEAPON_MESSAGE_IR_SIG, WEAPON_MESSAGE_FEEDBACK } weapon_receiving_message_type_t;

typedef enum {
    SLAVER_MESSAGE_NONE,
    SLAVER_MESSAGE_VIDEO,
    SLAVER_MESSAGE_MODEL,
    SLAVER_MESSAGE_INFO,
} slaver_board_message_type_t;

/**
 * \brief   �洢������������
 */
typedef struct {
    volatile uint16_t datanum;                /*!< �����ֽ��� */
    uint8_t uart_buff[BUFF_SIZE];             /*!< ���ݻ����� */
    uint8_t receive_data_flag;                /*!< ������ɱ�־λ */
    bt_receiving_message_type_t message_type; /*!< ��Ϣ���� */
} bt_received_data_t;

/**
 * \brief   �洢������������
 */
typedef struct {
    volatile uint16_t datanum;                    /*!< �����ֽ��� */
    uint8_t uart_buff[BUFF_SIZE];                 /*!< ���ݻ����� */
    uint8_t receive_data_flag;                    /*!< ������ɱ�־λ */
    weapon_receiving_message_type_t message_type; /*!< ��Ϣ���� */
} weapon_received_data_t;

/**
 * \brief   �Ӱ�������ݻ�����
 */
typedef struct {
    volatile uint16_t datanum;                /*!< �����ֽ��� */
    uint8_t uart_buff[BUFF_SIZE];             /*!< ���ݻ����� */
    uint8_t receive_data_flag;                /*!< ������ɱ�־λ */
    slaver_board_message_type_t message_type; /*!< ��Ϣ���� */
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
