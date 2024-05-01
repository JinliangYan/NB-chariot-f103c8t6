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

#include <stdlib.h>
#include <string.h>
#include "defence.h"
#include "slaver.h"
#include "state.h"
#include "usart.h"

/**
 * \brief 所有防具类型结构体数组, 以防具类型为index
 */
static const defence_t defences[10] = {
    [DEFENCE_TYPE_LIGHTWEIGHT] =
        {
            .type = DEFENCE_TYPE_LIGHTWEIGHT,
            .hp = HP_M,
            .weight = WEIGHT_M,
            .name = "DEFENCE_TYPE_LIGHTWEIGHT",
        },
    [DEFENCE_TYPE_HEAVYWEIGHT] =
        {
            .type = DEFENCE_TYPE_HEAVYWEIGHT,
            .hp = HP_L,
            .weight = WEIGHT_L,
            .name = "DEFENCE_TYPE_HEAVYWEIGHT",
        }
    // TODO 补充其他防具类型
};

/**
 * \brief 防具数据结构体
 */
defence_t defence;

static void clear_attacker_info(void);
static attacker_t attacker;

void
defence_init(void) {
    defence_type_t defence_type = DEFENCE_TYPE_BEGIN;

    /* 1. 扫描，确定类型 */
    for (defence_type_t i = defence_type; i < DEFENCE_TYPE_END; ++i) {
        if (slaver_model_addr_confirm("defence", i)) {
            defence_type = i;
            break;
        }
    }
    /* 设置默认值 */
    if (defence_type == DEFENCE_TYPE_BEGIN) {
        defence_type = DEFENCE_TYPE_LIGHTWEIGHT;
    }
    defence = defences[defence_type];
}

void
defence_control(void) {
    /* 从副板获得攻击者ID，伤害，技能 */
    if (slaver_received_data.receive_data_flag == 1 && slaver_received_data.message_type == SLAVER_MESSAGE_INFO) {

        if (strstr((char*)slaver_received_data.uart_buff, "attacked")
            && strstr((char*)slaver_received_data.uart_buff, "defence")) {

            sscanf(
                (char*)slaver_received_data.uart_buff,
                "attacked-i%d-s%d-p%d-defence",
                &attacker.id, &attacker.skill, &attacker.power
                );

            slaver_received_data.receive_data_flag = 0;
        }
        if (attacker.id > 0 && attacker.id <= 3 && attacker.id != CHARIOT_ID) {
            if (defence.hp < attacker.power) {
                defence.hp = 0;
            } else {
                defence.hp -= attacker.power;
            }

            state_update_info(MODEL_DEFENCE, ATTRIBUTE_HP, defence.hp);
            char cmd[20];
            sprintf(cmd, "dvh%d", (uint8_t)(1.0 * defence.hp / defences[defence.type].hp * 100));
            slaver_send('M', cmd);
            clear_attacker_info();
        }
    }
}

static char* get_attacker_info(char *string) {

}

static void
clear_attacker_info(void) {
    attacker.power = 0;
    attacker.id = 0;
    attacker.skill = 0;
}
