/**
 * \file            servo.c
 * \date            3/9/2024
 * \brief           武器舵机
 *  PA6 ---> 左右舵机
 *  PA7 ---> 上下舵机
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

#include "servo.h"
#include "printf.h"
#include "delay.h"
#include "pwm.h"

void
servo_init(void) {
    pwm3_init(20000, 72);
    servo_1_set_angle(90);
    servo_2_set_angle(90);
}

void
pwm_set_compare_1(uint16_t Compare) {
    TIM_SetCompare1(TIM3, Compare);
}

void
pwm_set_compare_2(uint16_t Compare) {
    TIM_SetCompare2(TIM3, Compare);
}

/**
 * \brief 设置舵机转角, 该舵机控制武器左右转向
 * \param[in] Angle:转向角, 范围0~180
 */
void
servo_1_set_angle(float Angle) {
    Angle = 180 - Angle;
    pwm_set_compare_1((uint16_t)(Angle / 180 * 2000) + 500);
    // printf_("set angle1 = %d\r\n", (uint16_t)Angle);
}

/**
 * \brief 设置舵机转角, 该舵机控制武器左右转向
 * \param[in] Angle:转向角, 范围0~180
 */
void
servo_2_set_angle(float Angle) {
    Angle = 180 - Angle;
    pwm_set_compare_2((uint16_t)(Angle / 180 * 2000) + 500);
    // printf_("set angle2 = %d\r\n", (uint16_t)Angle);
}