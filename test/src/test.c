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
#include "delay.h"
#include "hcsr.h"
#include "led.h"
#include "motor.h"
#include "move.h"
#include "printf.h"
#include "stm32f10x.h"
#include "ws2812b.h"
#include "blt.h"

void base_init(void);
void test_bt(void);
void test_weapon(void);
void test_motor_pwm(void);
void test_usart1(void);
void test_remap(void);
void test(void);
void test_hcsr();

void pin_remap(void);
void models_init(void);

int
debug_test(void) {
    pin_remap();
    base_init();
    models_init();
    test();
    printf_("HELLO\r\n");

    while (1) {
//        control();
//        dlc_control();
//        status_handler();
move_control();
    }

    return 0;
}

void
test(void) {
//    test_bt();
    test_motor_pwm();
//    test_usart1();
//    test_weapon();
//    test_remap();
//    test_hcsr();
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
    motor_state(1);
    forward(100);
    backward(100);
    motor_turn_left(100);

    move(0, 0);

}

void
test_usart1(void) {
    test_bt();
}

void
test_weapon(void) {
}

void
test_remap(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    PAout(15) = 0;
    delay_ms(1000);
    PAout(15) = 1;
    delay_ms(1000);
    PBout(3) = 0;
    delay_ms(1000);
    PBout(3) = 1;
    delay_ms(1000);
    PBout(4) = 0;
    delay_ms(1000);
    PBout(4) = 1;
    delay_ms(1000);
}

void
test_hcsr(void) {
    delay_init();
    hcsr04_init();
    Hcsr04_Text();
}
