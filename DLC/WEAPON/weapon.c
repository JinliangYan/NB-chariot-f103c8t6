/**
 * \file            weapon.c
 * \date            3/9/2024
 * \brief           武器模块,设计武器的转向,攻击, 技能释放
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
#include "defence.h"
#include "ir.h"
#include "servo.h"
#include "status.h"
#include "usart.h"

/**
 * \brief 手柄偏移量范围
 */
#define JOYSTICK_MIN (10)
#define JOYSTICK_MAX (90)

/**
 * \brief 舵机角度范围
 * \note  应保证 (SERVO_MIN + SERVO_MAX) / 2 == 90
 */
#define SERVO_X_MIN  (0)
#define SERVO_X_MAX  (180)
#define SERVO_Y_MIN  (50)
#define SERVO_Y_MAX  (130)

/**
 * \brief 所有技能类型结构体数组, 以技能类型为index
 */
static weapon_skill_t weapon_skills[256] = {
    [WEAPON_SKILL_INCREASE_DAMAGE] = {
        .skill_type = WEAPON_SKILL_INCREASE_DAMAGE,
        .activated_times = 2,
        .cooldown_time = 30,
        .duration = 30,
        .remaining_duration = 0
    },
    //TODO 补充其他技能类型
};

/**
 * \brief 所有武器类型结构体数组, 以武器类型为index
 */
static weapon_t weapons[256] = {
    [WEAPON_TYPE_GUN] = {
        .type = WEAPON_TYPE_GUN,
        .skill_type = WEAPON_SKILL_INCREASE_DAMAGE,
        .power = 50,
        .steerable = 1,
        .weight = 1},
    [WEAPON_TYPE_OTHER] = {
        .type = WEAPON_TYPE_OTHER,
        .skill_type = WEAPON_SKILL_INCREASE_DAMAGE,
        .power = 70,
        .steerable = 0,
        .weight = 1}
    // TODO 补充其他武器类型
};

/**
 * \brief 武器攻击者
 */
static attacker_t attacker;

/**
 * \brief 挂载的武器
 */
static weapon_t weapon;

/**
 * \brief 挂载的武器的技能
 */
static weapon_skill_t weapon_skill;

static void weapon_steering(void);
static void weapon_attack(void);
static void clear_attacker(void);
static uint8_t weapon_activate_skill(void);
static void bt_received_data_handler(void);
static void weapon_received_data_handler(void);

/**
 * \brief 武器初始化
 */
void
weapon_init(void) {
    //TODO 判断挂载的武器类型
    weapon_type_t weapon_type = WEAPON_TYPE_GUN;
    weapon = weapons[weapon_type];

    /* 武器初始化角度, 不可更改! */
    weapon.x_angle = 90;
    weapon.y_angle = 90;
    servo_init();

    /* 绑定武器技能 */
    weapon_skill = weapon_skills[weapon.skill_type];

    ir_init();
    clear_attacker();
}

/**
 * \brief 武器事件处理函数
 */
void
weapon_control_loop(void) {
    if (bt_received_data.receive_data_flag == 1) {
        bt_received_data_handler();
    }
    if (weapon_received_data.receive_data_flag == 1) {
        weapon_received_data_handler();
    }
}

/**
 * \brief 有关武器的蓝牙数据处理
 */
static void
bt_received_data_handler(void) {
    if (bt_received_data.receive_data_flag == 0) {
        return;
    }

    if (bt_received_data.message_type == BT_MESSAGE_WEAPON_JOYSTICK) {
        weapon_steering();
        bt_received_data.receive_data_flag = 0;
    } else if (bt_received_data.message_type == BT_MESSAGE_WEAPON_ATTACK) {
        weapon_attack();
        bt_received_data.receive_data_flag = 0;
    } else if (bt_received_data.message_type == BT_MESSAGE_WEAPON_SKILL) {
        weapon_activate_skill();
        bt_received_data.receive_data_flag = 0;
    }
}

/**
 * \brief 武器模块接收到的红外处理
 */
static void
weapon_received_data_handler(void) {
    if (weapon_received_data.receive_data_flag == 0) {
        return;
    }

    /* 获取攻击者信息 */
    attacker.id = weapon_received_data.uart_buff[0];
    attacker.skill = weapon_received_data.uart_buff[1];
    attacker.power = weapon_received_data.uart_buff[2];

    if (attacker.id != 0) {
        //TODO 判断该红外是否来自敌方小车
        chariot_status.weapon_hp -= attacker.power;
        clear_attacker();
        weapon_received_data.receive_data_flag = 0;
    }
}

/**
 * \brief 攻击
 * TODO 伤害值计算
 */
static void
weapon_attack(void) {
    ir_emission(CHARIOT_ID, 0, weapon.power);
}

/**
 * \brief   释放技能
 * \return  释放成功(0)失败(1)
 */
static uint8_t
weapon_activate_skill(void) {
    if (weapon_skill.activated_times <= 0 || weapon_skill.cooldown_time > 0) {
        /* 技能可释放次数不足或正在冷却, 无法释放 */
        //TODO 做出反应, 例如告诉小程序技能正在冷却, 也可以在小程序端计时
        return 1;
    }

    /* 更新技能状态 */
    weapon_skill.activated_times--;
    weapon_skill.remaining_duration = weapon_skill.duration;
    // TODO 需要通过计时器实现duration递减


    if (weapon_skill.skill_type == WEAPON_SKILL_INCREASE_DAMAGE) {
        /* 技能效果: 攻击力增加 */
        weapon.power = (uint8_t)(weapon.power * 1.5);
    }
    //TODO 补充其他技能效果

    return 0;
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
    *servo1_angle =
        SERVO_X_MIN + (SERVO_X_MAX - SERVO_X_MIN) * (x_offset - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN);

    // 将手柄的y偏移量映射到第二个舵机的角度范围
    *servo2_angle =
        SERVO_Y_MIN + (SERVO_Y_MAX - SERVO_Y_MIN) * (y_offset - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN);
}

/**
 * \brief 武器转向
 */
static void
weapon_steering(void) {
    float weapon_x = (float)((bt_received_data.uart_buff[1] - '0') * 10 + bt_received_data.uart_buff[2] - '0');
    float weapon_y = (float)((bt_received_data.uart_buff[4] - '0') * 10 + bt_received_data.uart_buff[5] - '0');
    float w_angle_x;
    float w_angle_y;
    map_joystick_to_servos(weapon_x, weapon_y, &w_angle_x, &w_angle_y);

    /* 指令方向与现在方向相同, 直接返回 */
    if (w_angle_x == weapon.x_angle && w_angle_y == weapon.y_angle) {
        return;
    }

    weapon.x_angle = w_angle_x;
    weapon.y_angle = w_angle_y;
    servo_1_set_angle(w_angle_x);
    servo_2_set_angle(w_angle_y);
}

static void
clear_attacker(void) {
    attacker.power = 0;
    attacker.skill = 0;
    attacker.id = 0;
}
