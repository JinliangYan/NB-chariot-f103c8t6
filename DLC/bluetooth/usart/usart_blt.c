/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   HC05��������
  ******************************************************************************
  */

#include "usart_blt.h"
#include <stdarg.h>

extern bt_received_data_t bt_received_data;

/***************** ����һ���ַ�  **********************/
static void
Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t ch) {
    /* ����һ���ֽ����ݵ�USART1 */
    USART_SendData(pUSARTx, ch);

    /* �ȴ�������� */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

/*****************  ����ָ�����ȵ��ַ��� **********************/
void
Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    } while (k < strlen);
}

/*****************  �����ַ��� **********************/
void
Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    } while (*(str + k) != '\0');
}

//��ȡ���յ������ݺͳ���
char*
get_rebuff(uint16_t* len) {
    *len = bt_received_data.datanum;
    return (char*)&bt_received_data.uart_buff;
}

//��ջ�����
void
clean_rebuff(void) {

    uint16_t i = UART_BUFF_SIZE + 1;
    bt_received_data.datanum = 0;
    bt_received_data.receive_data_flag = 0;
    bt_received_data.message_type = MESSAGE_NONE;
    while (i) {
        bt_received_data.uart_buff[--i] = 0;
    }
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
