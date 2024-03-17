#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include "stm32f10x.h"

/**
 * \brief   �������ڲ�����
 */
#define BLT_USART_BAUD_RATE 9600

/**
 * \brief   �������ڽ��ջ�������С
 */
#define UART_BUFF_SIZE      1024

/**
 * \brief   ���ݰ���ʼλ
 */
#define PACKAGE_START_FLAG  '$'

/**
 * \brief   ���ݰ�����λ
 */
#define PACKAGE_END_FLAG    '*'

/**
 * \brief   ATָ��ظ���ʼλ
 */
#define PACKAGE_AT_FLAG     '+'

/**
 * \brief   ����������Ϣ����
*/
typedef enum {
    MESSAGE_NONE,            /*!< Ĭ��ֵ */
    MESSAGE_MODE_SWITCH,     /*!< ģʽ�л����� */
    MESSAGE_LEFT_JOYSTICK,   /*!< ��ҡ������ */
    MESSAGE_RIGHT_JOYSTICK,  /*!< ��ҡ������ */
    MESSAGE_WEAPON_JOYSTICK, /*!< ����ҡ������ */
    MESSAGE_WEAPON_ATTACK,   /*!< ����������    �� */
    MESSAGE_TEXT,            /*!< �ı����� */
    MESSAGE_AT_COMMAND       /*!< ATָ��ظ� */
} receiving_message_type_t;

/**
 * \brief   �洢������������
 */
typedef struct {
    volatile uint16_t datanum;              /*!< �����ֽ��� */
    uint8_t uart_buff[UART_BUFF_SIZE];      /*!< ���ݻ����� */
    uint8_t receive_data_flag;              /*!< ������ɱ�־λ */
    receiving_message_type_t message_type;  /*!< ��Ϣ���� */
} bt_received_data_t;

void Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen);
void Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str);

char* get_rebuff(uint16_t* len);
void clean_rebuff(void);

#endif /* __USART2_H */
