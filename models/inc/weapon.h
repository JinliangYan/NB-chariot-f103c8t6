/**
 * \file            weapon.h
 * \date            3/9/2024
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

#ifndef NB_CHARIOT_F103C8T6_WEAPON_H
#define NB_CHARIOT_F103C8T6_WEAPON_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \brief   武器类型枚举
 */
typedef enum {
    WEAPON_TYPE_BEGIN = 0xA0,

    WEAPON_TYPE_GUN,
    WEAPON_TYPE_OTHER,
    // TODO 补充武器类型

    WEAPON_TYPE_END,
} weapon_type_t;

/**
 * \brief 技能种类枚举
 */
typedef enum {
    WEAPON_SKILL_INCREASE_DAMAGE, /* 增加攻击力 */
    WEAPON_SKILL_COUNT
    //TODO 补充技能类型
} skill_t;

typedef struct {
    skill_t skill_type;         /*!< 技能类型 */
    uint8_t cooldown_time;      /*!< 技能冷却时间 */
    uint8_t activated_times;    /*!< 技能可发动次数 */
    uint8_t duration;           /*!< 技能持续时间 */
    uint8_t remaining_duration; /*!< 技能剩余持续时间 */
    uint8_t state;              /*!< 是否激活 */
    char* name;
} weapon_skill_t;

typedef struct {
    weapon_type_t type; /*!< 武器类型 */
    skill_t skill_type; /*!< 技能类型 */
    uint8_t power;      /*!< 攻击力 */
    int32_t hp;         /*!< 血量 */
    uint8_t weight;     /*!< 重量 */
    uint8_t steerable;  /*!< 能(1)否(0)转向 */
    float x_angle;      /*!< 武器当前X偏移角, 当steerable为0时无用 */
    float y_angle;      /*!< 武器当前Y偏移角, 当steerable为0时无用 */
    char* name;
} weapon_t;

void weapon_init(void);
void weapon_control(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_WEAPON_H */
