/**
 * \file            test.c
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
#include "control.h"
#include "delay.h"
#include "dlc.h"
#include "hcsr.h"
#include "led.h"
#include "motor.h"
#include "printf.h"
#include "stm32f10x.h"
#include "ws2812b.h"

static uint8_t status_test;

void sysinit(void);
void test_bt(void);
void test_weapon(void);
void test_motor_pwm(void);
void test_usart1(void);
void test(void);

int
debug_test(void) {
    sysinit();
    test();
    printf_("HELLO\r\n");

    while (1) {
        control();
        dlc_control();
        status_test = status_check();
        if (status_test == STATUS_DEAD) {
            status_control(status_test);
            break;
        }
    }

    // TODO 死亡结算
    return 0;
}

void
sysinit(void) {
    /* 释放PB3、PB4、PA15引脚 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    delay_init(); //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    Motor_Init();      //电机初始化
    Hcsr04_Init();     //超声波初始化
    Motion_State(OFF); //关闭电机驱动失能
    RGB_LED_Init();    //RGB彩灯初始化
    printf_init();
    dlc_init();
    delay_ms(1000);
}

void
test(void) {
    // test_bt();
    //    test_motor_pwm();
    // test_usart1();
    test_weapon();
}

void
test_bt(void) {
    if (is_bt_connected()) {
        printf_("BT CONNECTED\r\n");
        return;
    }
    if (!bt_reset()) {
        printf_("PASS:bt_reset()\r\n");
    }
}

void
test_motor_pwm(void) {
    printf_("MOTOR TEST START\r\n");
    TIM_SetCompare1(TIM1, 300);
    TIM_SetCompare2(TIM1, 300);
    TIM_SetCompare3(TIM1, 300);
    TIM_SetCompare4(TIM1, 300);
}

void
test_usart1(void) {
    test_bt();
}

void
test_weapon(void) {
    weapon_attack(0);
}
