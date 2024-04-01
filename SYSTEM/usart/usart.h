#ifndef USART_H
#define USART_H
#include "stdio.h"
#include "sys.h"

/**
 * \brief   �������ڽ��ջ�������С
 */
#define UART_BUFF_SIZE      1024

/**
 * \brief   ����������Ϣ����
*/
typedef enum {
    MESSAGE_NONE,            /*!< Ĭ��ֵ */
    MESSAGE_MODE_SWITCH,     /*!< ģʽ�л����� */
    MESSAGE_LEFT_JOYSTICK,   /*!< ��ҡ������ */
    MESSAGE_RIGHT_JOYSTICK,  /*!< ��ҡ������ */
    MESSAGE_WEAPON_JOYSTICK, /*!< ����ҡ������ */
    MESSAGE_WEAPON_ATTACK,   /*!< ������������ */
    MESSAGE_WEAPON_SKILL,    /*!< ���������ͷ����� */
    MESSAGE_TEXT,            /*!< �ı����� */
    MESSAGE_AT_COMMAND       /*!< ATָ��ظ� */
} receiving_message_type_t;

/**
 * \brief   �洢������������
 */
typedef struct {
    volatile uint16_t datanum;             /*!< �����ֽ��� */
    uint8_t uart_buff[UART_BUFF_SIZE];     /*!< ���ݻ����� */
    uint8_t receive_data_flag;             /*!< ������ɱ�־λ */
    receiving_message_type_t message_type; /*!< ��Ϣ���� */
} bt_received_data_t;

/**
 * \brief   �洢������������
 */
typedef struct {
    volatile uint16_t datanum;         /*!< �����ֽ��� */
    uint8_t uart_buff[UART_BUFF_SIZE]; /*!< ���ݻ����� */
    uint8_t receive_data_flag;         /*!< ������ɱ�־λ */
} weapon_received_data_t;

extern bt_received_data_t bt_received_data;
extern weapon_received_data_t weapon_received_data;


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
void usart3_send_str(uint8_t* data);

#endif
