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

#include "stm32f10x.h"
#include "defence.h"
#include "weapon.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \brief ս��ID, 1 ~ 255
 */
#define CHARIOT_ID 0x01

#define WEIGHT_0   0
#define WEIGHT_S   50
#define WEIGHT_M   80
#define WEIGHT_L   110

typedef enum {
    CHARIOT_TYPE_LW,        /*!< ������ս���� */
    CHARIOT_TYPE_HW,
} chariot_type_t;

typedef struct {
    uint32_t core_hp;   /*!< ����Ѫ�� */
    uint8_t bt_connected;  /*!< ��������״̬ */
    uint32_t total_weight; /*!< ս�������� */
    weapon_t* weapon;      /*!< �������ݽṹ�� */
    defence_t* defence;     /*!< �������ݽṹ�� */
    weapon_skill_t* skill; /*!< �������ݽṹ�� */
} status_t;

extern status_t chariot;

void state_init(void);
void state_handler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_STATUS_H */
