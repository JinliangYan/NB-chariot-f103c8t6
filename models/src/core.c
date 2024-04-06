/**
* \file            core.c
* \date            4/5/2024
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

#include "core.h"
#include "irda.h"
#include "state.h"

static void clear_attacker_info(void);
uint8_t core_attacked_data[4];

static attacker_t attacker;

void core_init(void) {
    irda_init();
}

void
core_control(void) {
    if (irda_frame_flag == 1) {
        irda_process(core_attacked_data); /* 处理完后标志位置0了 */
        attacker.id = core_attacked_data[0];
        attacker.skill = core_attacked_data[1];
        attacker.power = core_attacked_data[2];
        /* 最后一个字节为power反码, 这里不取 */
    }
    if (attacker.id != 0) {
        chariot.core_hp -= attacker.power;
    }

    clear_attacker_info();
}

static void
clear_attacker_info(void) {
    attacker.power = 0;
    attacker.id = 0;
    attacker.skill = 0;
}
