/**
 * \file            status.h
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

#ifndef NB_CHARIOT_F103C8T6_STATUS_H
#define NB_CHARIOT_F103C8T6_STATUS_H

#include "defence.h"
#include "move.h"
#include "stm32f10x.h"
#include "weapon.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \brief 战车ID, 1 ~ 255
 */
#define CHARIOT_ID 0x01

#define WEIGHT_0   0
#define WEIGHT_S   50
#define WEIGHT_M   80
#define WEIGHT_L   110

#define HP_S       50
#define HP_M       100
#define HP_L       150

typedef enum {
    CHARIOT_TYPE_LW, /*!< 轻量型战车， */
    CHARIOT_TYPE_HW,
} chariot_type_t;

typedef enum {
    MODEL_CORE,
    MODEL_MOVE,
    MODEL_WEAPON,
    MODEL_DEFENCE,
    MODEL_SKILL,
} model_t;

typedef enum {
    ATTRIBUTE_HP,
    ATTRIBUTE_TIME,
    ATTRIBUTE_POWER,
} attribute_t;

typedef struct {
    char* name;
    int16_t core_hp;            /*!< 核心血量 */
    uint8_t bt_connected;       /*!< 蓝牙连接状态 */
    uint32_t total_weight;      /*!< 战车总重量 */
    move_model_t* move_model;   /*!< 移动模块数据结构体 */
    weapon_t* weapon;           /*!< 武器数据结构体 */
    defence_t* defence;         /*!< 防具数据结构体 */
    weapon_skill_t* skill;      /*!< 技能数据结构体 */
    uint16_t speed_with_weight; /*!< 计算重量后的速度 */
} status_t;


typedef struct {
    uint8_t id;
    uint8_t skill;
    uint8_t power;
} attacker_t;

extern status_t chariot;

void state_init(void);
void state_handler(void);
void state_update_model(model_t model, attribute_t attribute, uint8_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_STATUS_H */
