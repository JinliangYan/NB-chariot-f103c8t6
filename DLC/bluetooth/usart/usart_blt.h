#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include "stm32f10x.h"

/**
 * \brief   �������ڲ�����
 */
#define BLT_USART_BAUD_RATE 9600

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

void Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen);
void Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str);

char* get_rebuff(uint16_t* len);
void clean_rebuff(void);

#endif /* __USART2_H */
