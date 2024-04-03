/**
 * \file            communicate.c
 * \date            4/3/2024
 * \brief           与副板通信
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

#include "slaver.h"
#include "string.h"
#include "usart.h"

#define SLAVER_BAUD_RATE 9600

void
slaver_init(void) {
    usart3_init(SLAVER_BAUD_RATE);
    usart3_send_str("$VSTART*");
    while (slaver_received_data.receive_data_flag == 0 && strcmp((char*)slaver_received_data.uart_buff, "READY") != 0) {
        // 等待回应
    }
}

void
slaver_clean_rebuff(void) {
    uint16_t i = BUFF_SIZE + 1;
    slaver_received_data.datanum = 0;
    slaver_received_data.receive_data_flag = 0;
    slaver_received_data.message_type = SLAVER_MESSAGE_NONE;
    while (i) {
        slaver_received_data.uart_buff[--i] = 0;
    }
}
