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
#include "state.h"

//#define DEFENCE_DEBUG

/**
 * \brief ���з������ͽṹ������, �Է�������Ϊindex
 */
static const defence_t defences[256] = {
    [DEFENCE_TYPE_NONE] = {
        .type = DEFENCE_TYPE_NONE,
        .hp = 0,
        .weight = WEIGHT_0,
    },
    [DEFENCE_TYPE_LIGHTWEIGHT] = {
        .type = DEFENCE_TYPE_LIGHTWEIGHT,
        .hp = 2,
        .weight = WEIGHT_M,
    },
    [DEFENCE_TYPE_HEAVYWEIGHT] = {
        .type = DEFENCE_TYPE_HEAVYWEIGHT,
        .hp = 4,
        .weight = WEIGHT_L,
    }
    // TODO ����������������
};

/**
 * \brief �������ݽṹ��
 * \note  �����ģ��û���ҵ����ʵ�ʶ�𷽷�������ֻ����Ӧ�ö��ֶ�ѡ��
 */
defence_t defence;

static void clear_attacker_info(void);
uint8_t defence_attacked_data[4];
static attacker_t attacker;

void
defence_init(void) {
    irda_init();
    /* Ĭ��ʹ������������ */
    defence_type_t defence_type = DEFENCE_TYPE_LIGHTWEIGHT;
    
    // TODO ����û�ѡ��ķ�������
    
    defence = defences[defence_type];
}

void
defence_loop(void) {
    if (irda_frame_flag == 1) {
        irda_process(defence_attacked_data); /* ��������־λ��0�� */
        attacker.id = defence_attacked_data[0];
        attacker.skill = defence_attacked_data[1];
        attacker.power = defence_attacked_data[2];
        /* ���һ���ֽ�Ϊpower����, ���ﲻȡ */
    }
    if (attacker.id != 0) {
        if (defence.hp != 0) {
            if (attacker.power > defence.hp) {
                defence.hp = 0;
                defence.hp -= attacker.power - defence.hp;
            } else {
                defence.hp -= attacker.power;
            }
        } else {
            /* ���ı����� */
            chariot.core_hp = 0;
        }
#ifdef DEFENCE_DEBUG
        printf_("\r\n HP = %d \r\n", status_hp);
        printf_("\r\n attacker_id = %d \r\n", attacker.id);
        printf_("\r\n attacker_skill = %d \r\n", attacker.skill);
        printf_("\r\n attacker_power = %d \r\n", attacker.power);
#endif
    }

    clear_attacker_info();
}

static void
clear_attacker_info(void) {
    attacker.power = 0;
    attacker.id = 0;
    attacker.skill = 0;
}
