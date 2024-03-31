/**
 * \file            weapon.c
 * \date            3/9/2024
 * \brief           ����ģ��,���������ת��,����
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
#include "usart_blt.h"
#include "ir.h"
#include "status.h"

extern bt_received_data_t bt_received_data;

/**
 * \brief ����������, 0~255
 */
uint8_t weapon_power = 100;

/**
 * \brief �ֱ�ƫ������Χ
 */
#define JOYSTICK_MIN (10)
#define JOYSTICK_MAX (90)

/**
 * \brief ����Ƕȷ�Χ
 * \note  Ӧ��֤ (SERVO_MIN + SERVO_MAX) / 2 == 90
 */
#define SERVO_X_MIN  (0)
#define SERVO_X_MAX  (180)
#define SERVO_Y_MIN  (50)
#define SERVO_Y_MAX  (130)

/**
 * \brief �����������ݽ��ջ�����
 * \ref usart.c
 */
extern char Weapon_Buf[10];

/**
 * \brief ������ǰ����
 */
float weapon_x_angle = 90;
float weapon_y_angle = 90;

void weapon_steering();

/**
 * \brief ������ʼ��
 */
void
weapon_init() {
    servo_init();
    ir_init();
}

/**
 * \brief ����
 * \param[in,out]   skill:���ܲ���, �ɸ��ݸò����������˺�, ���߷������Է�ʩ��debuff(����и�Ч���Ļ�)
 * TODO �˺�ֵ����
 */
void
weapon_attack(uint8_t skill) {
    ir_emission(CHARIOT_ID, skill, weapon_power);
}

/**
 * \brief ��ҡ�˵�ƫ����ת��Ϊ����Ƕ�
 * \param x_offset ҡ��x��ƫ����
 * \param y_offset ҡ��y��ƫ����
 * \param servo1_angle ���1(x)�ĽǶ�
 * \param servo2_angle ���2(y)�ĽǶ�
 */
static void
map_joystick_to_servos(float x_offset, float y_offset, float* servo1_angle, float* servo2_angle) {
    // ���ֱ�ҡ��λ�ý�������
    x_offset = (x_offset < JOYSTICK_MIN) ? JOYSTICK_MIN : (x_offset > JOYSTICK_MAX) ? JOYSTICK_MAX : x_offset;
    y_offset = (y_offset < JOYSTICK_MIN) ? JOYSTICK_MIN : (y_offset > JOYSTICK_MAX) ? JOYSTICK_MAX : y_offset;

    // ���ֱ���xƫ����ӳ�䵽��һ������ĽǶȷ�Χ
    *servo1_angle =
        SERVO_X_MIN + (SERVO_X_MAX - SERVO_X_MIN) * (x_offset - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN);

    // ���ֱ���yƫ����ӳ�䵽�ڶ�������ĽǶȷ�Χ
    *servo2_angle =
        SERVO_Y_MIN + (SERVO_Y_MAX - SERVO_Y_MIN) * (y_offset - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN);
}

/**
 * \brief ����ת��
 */
void
weapon_steering() {
    float weapon_x = (float)((bt_received_data.uart_buff[1] - '0') * 10 + bt_received_data.uart_buff[2] - '0');
    float weapon_y = (float)((bt_received_data.uart_buff[4] - '0') * 10 + bt_received_data.uart_buff[5] - '0');
    float w_angle_x;
    float w_angle_y;
    map_joystick_to_servos(weapon_x, weapon_y, &w_angle_x, &w_angle_y);

    /* ָ��������ڷ�����ͬ, ֱ�ӷ��� */
    if (w_angle_x == weapon_x_angle && w_angle_y == weapon_y_angle) {
        return;
    }

    weapon_x_angle = w_angle_x;
    weapon_y_angle = w_angle_y;
    servo_1_set_angle(w_angle_x);
    servo_2_set_angle(w_angle_y);
}

void
weapon_control_loop() {
    if (bt_received_data.receive_data_flag == 0) {
        return;
    }

    if (bt_received_data.message_type == MESSAGE_WEAPON_JOYSTICK) {
        weapon_steering();
        bt_received_data.receive_data_flag = 0;
    } else if (bt_received_data.message_type == MESSAGE_WEAPON_ATTACK) {
        // TODO ����������
        uint8_t skill = 0;
        weapon_attack(skill);
    }
}
