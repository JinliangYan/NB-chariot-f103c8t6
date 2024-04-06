/**
 * \file            defence.h
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

#ifndef NB_CHARIOT_F103C8T6_DEFENCE_H
#define NB_CHARIOT_F103C8T6_DEFENCE_H

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
    DEFENCE_TYPE_BEGIN,

    DEFENCE_TYPE_NONE,          /*!< 无防具 */
    DEFENCE_TYPE_LIGHTWEIGHT,   /*!< 轻量级防具 */
    DEFENCE_TYPE_HEAVYWEIGHT,   /*!< 重量级防具 */

    DEFENCE_TYPE_END,
} defence_type_t;

typedef struct {
    defence_type_t type; /*!< 防具类型 */
    uint8_t hp;          /*!< 攻击力 */
    uint8_t weight;      /*!< 重量 */
} defence_t;

typedef struct {
    uint8_t id;
    uint8_t skill;
    uint8_t power;
} attacker_t;

void defence_init(void);
void defence_control(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_DEFENCE_H */
