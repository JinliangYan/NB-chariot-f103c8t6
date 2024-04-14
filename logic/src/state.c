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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blt.h"
#include "electromagnet.h"
#include "led.h"
#include "motor.h"
#include "slaver.h"
#include "state.h"
#include "usart.h"

extern move_model_t move_model;
extern weapon_t weapon;
extern defence_t defence;
extern weapon_skill_t weapon_skill;

chariot_t chariot;

static void chariot_hp_handler(void);
static void defence_hp_handler(void);
static void weapon_hp_handler(void);
static void bt_connect_handler(void);
static void game_handler(void);

void
state_init(void) {
    chariot.core_hp = 1;
    chariot.id = CHARIOT_ID;
    chariot.bt_connected = is_bt_connected();
    chariot.move_model = &move_model;
    chariot.weapon = &weapon;
    chariot.defence = &defence;
    chariot.skill = &weapon_skill;
    chariot.total_weight = weapon.weight + defence.weight;
    chariot.speed_with_weight = move_model.speed - chariot.total_weight;

    state_update_info(MODEL_CHARIOT, ATTRIBUTE_ID, chariot.id);
    state_update_info(MODEL_CORE, ATTRIBUTE_HP, chariot.core_hp);
    state_update_info(MODEL_MOVE, ATTRIBUTE_HP, chariot.move_model->hp);
//    state_update_info(MODEL_WEAPON, ATTRIBUTE_HP, chariot.weapon->hp);
    state_update_info(MODEL_DEFENCE, ATTRIBUTE_HP, chariot.defence->hp);
    state_update_info(MODEL_SKILL, ATTRIBUTE_ID, chariot.weapon->skill_type);
    state_update_info(MODEL_SKILL, ATTRIBUTE_TIME, chariot.skill->cooldown_time);
}

void
state_handler(void) {
    bt_connect_handler();
    chariot_hp_handler();
    defence_hp_handler();
    weapon_hp_handler();
    game_handler();
}

void
state_update_info(model_t model, attribute_t attribute, uint8_t value) {
    char state_str[256];

    if (model == MODEL_CORE) {
        if (attribute == ATTRIBUTE_HP) {
            /* 核心血量 */
            sprintf(state_str, "ch%d", value);
            bt_send20_packet(state_str);
        }
    } else if (model == MODEL_DEFENCE) {
        if (attribute == ATTRIBUTE_HP) {
            /* 防御模块血量 */
            sprintf(state_str, "mvh%d", value);
            bt_send20_packet(state_str);
        }
    } else if (model == MODEL_MOVE) {
        if (attribute == ATTRIBUTE_HP) {
            /* 行走模块血量 */
            sprintf(state_str, "mvh%d", value);
            bt_send20_packet(state_str);
        }
    } else if (model == MODEL_WEAPON) {
        if (attribute == ATTRIBUTE_HP) {
            /* 行走模块血量 */
            sprintf(state_str, "wph%d", value);
            bt_send20_packet(state_str);
        }
    } else if (model == MODEL_SKILL) {
        if (attribute == ATTRIBUTE_TIME) {
            /* 技能冷却时间 */
            sprintf(state_str, "scdt%d", value);
            bt_send20_packet(state_str);
        } else if (attribute == ATTRIBUTE_ID) {
            /* 技能类型
             * 1:加攻击力
             * 2:加速
             * 为什么是G开头? 因为队友要求
             */
            sprintf(state_str, "Gtype%d", value);
            bt_send20_packet(state_str);
        }
    } else if (model == MODEL_CHARIOT) {
        if (attribute == ATTRIBUTE_ID) {
            sprintf(state_str, "id%d", value);
            bt_send20_packet(state_str);
        }
    }
}

/**
 * \brief 检查蓝牙是否连接, 如果没有连接则循环等待，LED闪烁
 */
static void
bt_connect_handler(void) {
    if (is_bt_connected()) {
        LED = 0;
    }
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
        /* 关闭电机 */
        motor_state(0);
        slaver_send('I', "dead");
        exit(0);
    }
}

static void
defence_hp_handler(void) {
    if (chariot.defence->hp <= 0) {
        // TODO 防御模块被击破反应, 待完善
        /* 防具掉落 */
        electromagnet_off();
    }
}

static void
weapon_hp_handler(void) {
    if (chariot.weapon->hp <= 0) {
        // TODO 武器模块被击破反应, 待完善
    }
}

static void
game_handler(void) {
    if (bt_received_data.message_type == BT_MESSAGE_GAME && strstr((const char*)bt_received_data.uart_buff, "Death")) {
        /* 游戏时间到 */
        /* 关闭电机 */
        motor_state(0);
        exit(0);
    }
}
