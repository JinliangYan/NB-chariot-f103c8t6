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

#define IR_BAUD_RATE        9600
#define IR_ADDRESS          0xFA

#define IR_STAT_EMISSION    0xF1
#define IR_STAT_CHANGE_ADDR 0xF2
#define IR_STAT_BAUD_RATE   0xF3

#define IR_BAUD_CODE_4800   0x01
#define IR_BAUD_CODE_9600   0x02
#define IR_BAUD_CODE_19200  0x03
#define IR_BAUD_CODE_57600  0x04

void ir_init();

void ir_emission(uint8_t high, uint8_t low, uint8_t command);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_IR_H */
