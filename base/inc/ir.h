/**
 * \file            ir.h
 * \date            3/19/2024
 * \brief           
 */

/*
 * Copyright (c) 2024 JinLiang YAN
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of NB-chariot-f103c8t6.
 *
 * Author:          JinLiang YAN <yanmiku0206@outlook.com>
 */

#ifndef NB_CHARIOT_F103C8T6_IR_H
#define NB_CHARIOT_F103C8T6_IR_H

#include "stm32f10x.h"
#include "usart.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    IR_STATE_EMISSION = 0xF1,
    IR_STATE_CHANGE_ADDR,
    IR_STATE_BAUD_RATE,
} ir_state_t;

typedef enum {
    IR_BAUD_RATE_4800 = 0x01,
    IR_BAUD_RATE_9600,
    IR_BAUD_RATE_19200,
    IR_BAUD_RATE_57600,
} ir_baud_rate_t;

void ir_init(void);
void ir_emission(uint8_t ir_addr, uint8_t high, uint8_t low, uint8_t command);
char* ir_change_addr(uint8_t ir_addr, uint8_t new_addr);
char* ir_change_baud_rate(uint8_t ir_addr, ir_baud_rate_t ir_baud_rate);
uint8_t ir_addr_confirm(uint8_t ir_addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_IR_H */
