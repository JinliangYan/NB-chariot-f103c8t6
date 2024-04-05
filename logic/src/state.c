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
#include "state.h"
#include "blt.h"
#include "electromagnet.h"
#include "led.h"
#include "motor.h"

status_t chariot;

static void chariot_hp_handler(void);
static void defence_hp_handler(void);
static void weapon_hp_handler(void);
static void bt_connect_handler(void);

void
state_init(void) {
    chariot.core_hp = 1;
    chariot.bt_connected = is_bt_connected();
}

/**
 * \brief ��������Ƿ�����, ���û���������ڴ�ѭ��
 */
void
state_handler(void) {
    bt_connect_handler();
    chariot_hp_handler();
    defence_hp_handler();
    weapon_hp_handler();
}

static void
bt_connect_handler(void) {
    while ((chariot.bt_connected = is_bt_connected()) != 1) {
        LED = 1;
        delay_ms(500);
        LED = 0;
        delay_ms(500);
    }
}

static void
chariot_hp_handler(void) {
    if (chariot.core_hp <= 0) {
        // TODO ��Ϸ����, ��β����, ������
        motor_state(0);
        exit(0);
    }
}

static void
defence_hp_handler(void) {
    if (chariot.defence->hp <= 0) {
        // TODO ����ģ�鱻���Ʒ�Ӧ, ������
    }
}

static void
weapon_hp_handler(void) {
    if (chariot.weapon->hp <= 0) {
        // TODO ����ģ�鱻���Ʒ�Ӧ, ������
        /* �������� */
        electromagnet_off();
    }
}