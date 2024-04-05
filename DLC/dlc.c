/**
 * \file            dlc_control.c
 * \date            3/10/2024
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

#include "dlc.h"
#include "timer.h"

/**
 * \brief 计时变量
 */
uint32_t count_1ms;
uint32_t count_2ms;
uint32_t count_4ms;
uint32_t count_10ms;
uint32_t count_100ms;
uint32_t count_500ms;
uint32_t count_1000ms;

extern weapon_skill_t weapon_skill;

void
dlc_init(void) {
    tim2_init(72, 1000);
    state_init();
    electromagnet_init();
    weapon_init();
    defence_init();
    bt_init();
}

void
dlc_control(void) {
    weapon_control_loop();
    defence_loop();
}

/**
 * \brief 对时间要求高的任务调度
 */
__attribute__((unused)) void
TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        count_1ms++;
        count_2ms++;
        count_4ms++;
        count_10ms++;
        count_100ms++;
        count_500ms++;
        count_1000ms++;

        if (count_1ms == 1) {
            count_1ms = 0;
        }
        if (count_2ms == 2) {
            count_2ms = 0;
        }
        if (count_4ms == 4) {
            count_4ms = 0;
        }
        if (count_10ms == 10) {
            count_10ms = 0;
        }
        if (count_100ms == 100) {
            count_100ms = 0;
        }
        if (count_500ms == 500) {
            count_500ms = 0;
        }
        if (count_1000ms == 1000) {
            if (weapon_skill.remaining_duration > 0) {
                weapon_skill.remaining_duration--;
            }
            count_1000ms = 0;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除标志位
    }
}
