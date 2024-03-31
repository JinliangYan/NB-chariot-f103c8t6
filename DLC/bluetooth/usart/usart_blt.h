#ifndef __USART2_H
#define __USART2_H

#include <stdio.h>
#include "stm32f10x.h"

/**
 * \brief   蓝牙串口波特率
 */
#define BLT_USART_BAUD_RATE 9600

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

void Usart_SendStr_length(USART_TypeDef* pUSARTx, uint8_t* str, uint32_t strlen);
void Usart_SendString(USART_TypeDef* pUSARTx, uint8_t* str);

char* get_rebuff(uint16_t* len);
void clean_rebuff(void);

#endif /* __USART2_H */
