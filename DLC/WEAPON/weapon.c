/**
 * \file            weapon.c
 * \date            3/9/2024
 * \brief           武器模块,设计武器的转向,攻击
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
#include "weapon.h"
#include "printf.h"
#include "servo.h"

/**
 * \brief 手柄偏移量范围
 */
#define JOYSTICK_MIN             (10)
#define JOYSTICK_MAX             (90)

/**
 * \brief 舵机角度范围
 * \note  应保证 (SERVO_MIN + SERVO_MAX) / 2 == 90
 */
#define SERVO_X_MIN                (0)
#define SERVO_X_MAX                (180)
#define SERVO_Y_MIN                (50)
#define SERVO_Y_MAX                (130)

/**
 * \brief 武器方向数据接收缓冲区
 * \ref usart.c
 */
extern char Weapon_Buf[10];

/**
 * \brief 武器当前方向
 */
float weapon_x_angle = 90;
float weapon_y_angle = 90;

void weapon_steering();

/**
 * \brief 武器初始化
 */
void
weapon_init() {
    servo_init();
    servo_1_set_angle(90);
    servo_2_set_angle(90);
//    printf_("weapon initialized successful\r\n");
}

/**
 * \brief 攻击
 */
void
weapon_attack() {
    // TODO ATTACK LOGIC
//    printf_("weapon attack\r\n");
}


/**
 * \brief 将摇杆的偏移量转换为舵机角度
 * \param x_offset 摇杆x的偏移量
 * \param y_offset 摇杆y的偏移量
 * \param servo1_angle 舵机1(x)的角度
 * \param servo2_angle 舵机2(y)的角度
 */
static void
map_joystick_to_servos(float x_offset, float y_offset, float* servo1_angle, float* servo2_angle) {
    // 对手柄摇杆位置进行限制
    x_offset = (x_offset < JOYSTICK_MIN) ? JOYSTICK_MIN : (x_offset > JOYSTICK_MAX) ? JOYSTICK_MAX : x_offset;
    y_offset = (y_offset < JOYSTICK_MIN) ? JOYSTICK_MIN : (y_offset > JOYSTICK_MAX) ? JOYSTICK_MAX : y_offset;

    // 将手柄的x偏移量映射到第一个舵机的角度范围
    *servo1_angle = SERVO_X_MIN + (SERVO_X_MAX - SERVO_X_MIN) * (x_offset - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN);

    // 将手柄的y偏移量映射到第二个舵机的角度范围
    *servo2_angle = SERVO_Y_MIN + (SERVO_Y_MAX - SERVO_Y_MIN) * (y_offset - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN);
}

/**
 * \brief 武器转向
 */
void
weapon_steering() {
    float weapon_x = (float )((Weapon_Buf[2] - '0') * 10 + Weapon_Buf[3] - '0');
    float weapon_y = (float )((Weapon_Buf[5] - '0') * 10 + Weapon_Buf[6] - '0');
    float w_angle_x;
    float w_angle_y;
    map_joystick_to_servos(weapon_x, weapon_y, &w_angle_x, &w_angle_y);

    /* 指令方向与现在方向相同, 直接返回 */
    if (w_angle_x == weapon_x_angle && w_angle_y == weapon_y_angle)
        return;

    weapon_x_angle = w_angle_x;
    weapon_y_angle = w_angle_y;
//    printf_("%.2f --> %.2f\r\n", weapon_x, w_angle_x);
//    printf_("%.2f --> %.2f\r\n", weapon_y, w_angle_y);
    servo_1_set_angle(w_angle_x);
    servo_2_set_angle(w_angle_y);
}

void
weapon_control_loop() {
    if (Weapon_Buf[1] == 'X') {
        weapon_steering();
    } else if (Weapon_Buf[1] == 'A') {
        weapon_attack();
    }
}

