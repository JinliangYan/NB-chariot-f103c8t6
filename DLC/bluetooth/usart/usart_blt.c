/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   HC05串口驱动
  ******************************************************************************
  */

#include "usart_blt.h"
#include <stdarg.h>

extern bt_received_data_t bt_received_data;

/***************** 发送一个字符  **********************/
static void
Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t ch) {
    /* 发送一个字节数据到USART1 */
    USART_SendData(pUSARTx, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

/*****************  发送指定长度的字符串 **********************/
void
Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    } while (k < strlen);
}

/*****************  发送字符串 **********************/
void
Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    } while (*(str + k) != '\0');
}

//获取接收到的数据和长度
char*
get_rebuff(uint16_t* len) {
    *len = bt_received_data.datanum;
    return (char*)&bt_received_data.uart_buff;
}

//清空缓冲区
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
