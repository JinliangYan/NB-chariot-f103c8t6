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
 * \brief   ��������ö��
 */
typedef enum {
    WEAPON_TYPE_BEGIN = 0xA0,

    WEAPON_TYPE_GUN,
    WEAPON_TYPE_OTHER,
    // TODO ������������

    WEAPON_TYPE_END,
} weapon_type_t;

/**
 * \brief ��������ö��
 */
typedef enum {
    WEAPON_SKILL_INCREASE_DAMAGE, /* ���ӹ����� */
    WEAPON_SKILL_COUNT
    //TODO ���似������
} skill_t;

typedef struct {
    skill_t skill_type;         /*!< �������� */
    uint8_t cooldown_time;      /*!< ������ȴʱ�� */
    uint8_t activated_times;    /*!< ���ܿɷ������� */
    uint8_t duration;           /*!< ���ܳ���ʱ�� */
    uint8_t remaining_duration; /*!< ����ʣ�����ʱ�� */
    uint8_t state;              /*!< �Ƿ񼤻� */
    char* name;
} weapon_skill_t;

typedef struct {
    weapon_type_t type; /*!< �������� */
    skill_t skill_type; /*!< �������� */
    uint8_t power;      /*!< ������ */
    int32_t hp;         /*!< Ѫ�� */
    uint8_t weight;     /*!< ���� */
    uint8_t steerable;  /*!< ��(1)��(0)ת�� */
    float x_angle;      /*!< ������ǰXƫ�ƽ�, ��steerableΪ0ʱ���� */
    float y_angle;      /*!< ������ǰYƫ�ƽ�, ��steerableΪ0ʱ���� */
    char* name;
} weapon_t;

void weapon_init(void);
void weapon_control(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_WEAPON_H */
