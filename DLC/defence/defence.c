/**
 * \file            defence.c
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

#include "defence.h"
#include "irda.h"
#include "printf.h"
#include "status.h"

#define DEFENCE_DEBUG

static void clear_attacker_info(void);
uint8_t defence_attacked_data[4];
attacker_t attacker;

void
defence_init(void) {
    irda_init();
}

void
defence_loop(void) {
    if (irda_frame_flag == 1) {
        irda_process(defence_attacked_data); /* 处理完后标志位置0了 */
        attacker.id = defence_attacked_data[0];
        attacker.skill = defence_attacked_data[1];
        attacker.power = defence_attacked_data[2];
        /* 最后一个字节为power反码, 这里不取 */
    }
    if (attacker.id) {
        status_hp -= attacker.power;
        clear_attacker_info();
#ifdef DEFENCE_DEBUG
        printf_("\r\n HP = %d \r\n", status_hp);
        printf_("\r\n attacker_id = %d \r\n", attacker.id);
        printf_("\r\n attacker_skill = %d \r\n", attacker.skill);
        printf_("\r\n attacker_power = %d \r\n", attacker.power);
#endif
    }
}

static void
clear_attacker_info(void) {
    attacker.power = 0;
    attacker.id = 0;
    attacker.skill = 0;
}
