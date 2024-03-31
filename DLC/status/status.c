/**
 * \file            status.c
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

#include <stdlib.h>
#include "electromagnet.h"
#include "motor.h"
#include "status.h"

status_t chariot_status;

static void chariot_hp_handler(void);
static void defence_hp_handler(void);
static void weapon_hp_handler(void);

void
status_init(void) {
    chariot_status.chariot_hp = 100;
    chariot_status.weapon_hp = 100;
    chariot_status.defence_hp = 100;
}

void
status_handler(void) {
    chariot_hp_handler();
    defence_hp_handler();
    weapon_hp_handler();
}

static void
chariot_hp_handler(void) {
    if (chariot_status.chariot_hp <= 0) {
        // TODO 游戏结束, 收尾任务, 待完善
        Motion_State(OFF);
        exit(0);
    }
}

static void
defence_hp_handler(void) {
    if (chariot_status.defence_hp <= 0) {
        // TODO 防御模块被击破反应, 待完善
    }
}

static void
weapon_hp_handler(void) {
    if (chariot_status.weapon_hp <= 0) {
        // TODO 武器模块被击破反应, 待完善
        /* 武器掉落 */
        electromagnet_off();
    }
}
