/**
 * \file            weapon.c
 * \date            3/9/2024
 * \brief           ����ģ��,���������ת��,����, �����ͷ�
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
#include "state.h"
#include "slaver.h"
#include "ir.h"
#include "servo.h"
#include "usart.h"

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
 * \brief ���м������ͽṹ������, �Լ�������Ϊindex
 */
static const weapon_skill_t weapon_skills[10] = {
    [WEAPON_SKILL_INCREASE_DAMAGE] = {
        .skill_type = WEAPON_SKILL_INCREASE_DAMAGE,
        .activated_times = 2,
        .cooldown_time = 30,
        .remaining_cooldown_time = 0,
        .duration = 30,
        .remaining_duration = 0,
        .state = 0,
        .name = "WEAPON_SKILL_INCREASE_DAMAGE",
    },
    [WEAPON_SKILL_SPEED_UP] = {
        .skill_type = WEAPON_SKILL_SPEED_UP,
        .activated_times = 2,
        .cooldown_time = 30,
        .remaining_cooldown_time = 0,
        .remaining_duration = 0,
        .state = 0,
        .name = "WEAPON_SKILL_SPEED_UP",
    }
};

/**
 * \brief �����������ͽṹ������, ����������Ϊindex
 */
static const weapon_t weapons[10] = {
    [WEAPON_TYPE_HEAVY] = {
        .type = WEAPON_TYPE_HEAVY,
        .hp = HP_M,
        .skill_type = WEAPON_SKILL_INCREASE_DAMAGE,
        .power = 50,
        .steerable = 1,
        .weight = WEIGHT_M,
        .name = "WEAPON_TYPE_HEAVY",
    },
    [WEAPON_TYPE_LIGHTWEIGHT] = {
        .type = WEAPON_TYPE_LIGHTWEIGHT,
        .hp = HP_M,
        .skill_type = WEAPON_SKILL_INCREASE_DAMAGE,
        .power = 70,
        .steerable = 0,
        .weight = WEIGHT_L,
        .name = "WEAPON_TYPE_OTHER",
    }
    // TODO ����������������
};

/**
 * \brief ����������
 */
static attacker_t attacker;

/**
 * \brief ���ص�����
 * \note  ������Ϊ����ĵ�ַ
 */
weapon_t weapon;

/**
 * \brief ��������Ҫʹ�õ�
 */
extern move_model_t move_model;
extern move_model_t move_models[];

/**
 * \brief ���ص������ļ���
 */
weapon_skill_t weapon_skill;

static void weapon_steering(void);
static void weapon_attack(uint8_t charged);
static void clear_attacker(void);
static uint8_t weapon_activate_skill(void);
static void weapon_deactivate_skill(void);
static void bt_received_data_handler(void);
static void weapon_received_data_handler(void);

/**
 * \brief ������ʼ��
 */
void
weapon_init(void) {
    weapon_type_t weapon_type = WEAPON_TYPE_BEGIN;

    ir_init();

    /* ɨ�������⣬ȷ���������� */
    for (weapon_type_t i = weapon_type; i < WEAPON_TYPE_END; ++i) {
        if (ir_addr_confirm(i)) {
            weapon_type = i;
            break;
        }
    }

    /* ����Ĭ��ֵ */
    if (weapon_type == WEAPON_TYPE_BEGIN) {
        weapon_type = WEAPON_TYPE_HEAVY;
    }

    weapon = weapons[weapon_type];

    /* ������ʼ���Ƕ�, ���ɸ���! */
    weapon.x_angle = 90;
    weapon.y_angle = 90;
    servo_init();

    /* ���������� */
    weapon_skill = weapon_skills[weapon.skill_type];

    clear_attacker();
}

/**
 * \brief �����¼�������
 */
void
weapon_control(void) {
    if (weapon.hp <= 0) {
        return;     /* ������� */
    }
    if (bt_received_data.receive_data_flag == 1) {
        bt_received_data_handler();
    }
    if (weapon_received_data.receive_data_flag == 1) {
        weapon_received_data_handler();
    }
    /* ���ܳ���ʱ��������Ƴ����� */
    if (weapon_skill.state == 1 && weapon_skill.remaining_duration <= 0) {
        weapon_deactivate_skill();
    }
}

/**
 * \brief �й��������������ݴ���
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
        if (bt_received_data.uart_buff[0] == 'N') {
            weapon_attack(0);
        } else if (bt_received_data.uart_buff[0] == 'C') {
            weapon_attack(1);
        }
        bt_received_data.receive_data_flag = 0;
    } else if (bt_received_data.message_type == BT_MESSAGE_WEAPON_SKILL) {
        weapon_activate_skill();
        bt_received_data.receive_data_flag = 0;
    }
}

/**
 * \brief ����ģ����յ��ĺ��⴦��
 */
static void
weapon_received_data_handler(void) {
    if (weapon_received_data.receive_data_flag == 0) {
        return;
    }

    /* ��ȡ��������Ϣ */
    attacker.id = weapon_received_data.uart_buff[0];
    attacker.skill = weapon_received_data.uart_buff[1];
    attacker.power = weapon_received_data.uart_buff[2];

    if (attacker.id != 0) {
        //TODO �жϸú����Ƿ����Եз�С��
        weapon.hp -= attacker.power;

        if (weapon.hp < 0) {
            weapon.hp = 0;
        }
        state_update_model(MODEL_WEAPON, ATTRIBUTE_HP, weapon.hp);

        clear_attacker();
        weapon_received_data.receive_data_flag = 0;
    }
}

/**
 * \brief: ����
 * \param charged[in]: ��(1)��(0)����
 */
static void
weapon_attack(uint8_t charged) {
    char cmd[256];
    sprintf(cmd, "weaponattack-t%c", charged ? 'c' : 'n');
    slaver_send('I', cmd);
    ir_emission(weapon.type, CHARIOT_ID, 0, charged ? (uint8_t)(weapon.power * 1.5) : weapon.power);
}

/**
 * \brief   �ͷż���
 * \return  �ͷųɹ�(0)ʧ��(1)
 */
static uint8_t
weapon_activate_skill(void) {
    if (weapon_skill.activated_times <= 0 || weapon_skill.remaining_cooldown_time > 0) {
        /* ���ܿ��ͷŴ��������������ȴ, �޷��ͷ� */
        return 1;
    }

    /* ���¼���״̬ */
    weapon_skill.activated_times--;
    weapon_skill.remaining_duration = weapon_skill.duration;
    weapon_skill.remaining_cooldown_time = weapon_skill.cooldown_time;
    weapon_skill.state = 1;
    /* ͨ����ʱ��ʵ����remaining_time�ݼ� */

    if (weapon_skill.skill_type == WEAPON_SKILL_INCREASE_DAMAGE) {
        /* ����Ч��: ���������� */
        weapon.power = (uint8_t)(weapon.power * 1.5);
        slaver_send('I', "skill-t1");
    } else if (weapon_skill.skill_type == WEAPON_SKILL_SPEED_UP) {
        /* ����Ч��: �ƶ��ٶ����� */
        move_model.speed += 100;
        slaver_send('I', "skill-t2");
    }

    return 0;
}

/**
 * \brief �Ƴ�����Ч��
 */
static void
weapon_deactivate_skill(void) {
    if (weapon_skill.skill_type == WEAPON_SKILL_INCREASE_DAMAGE) {
        /* �Ƴ����������� */
        weapon.power = weapons[weapon.type].power;
    } else if (weapon_skill.skill_type == WEAPON_SKILL_SPEED_UP) {
        move_model.speed = move_models[move_model.type].speed;
    }
    weapon_skill.state = 0;
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
static void
weapon_steering(void) {
    float weapon_x = (float)((bt_received_data.uart_buff[1] - '0') * 10 + bt_received_data.uart_buff[2] - '0');
    float weapon_y = (float)((bt_received_data.uart_buff[4] - '0') * 10 + bt_received_data.uart_buff[5] - '0');
    float w_angle_x;
    float w_angle_y;
    map_joystick_to_servos(weapon_x, weapon_y, &w_angle_x, &w_angle_y);

    /* ָ��������ڷ�����ͬ, ֱ�ӷ��� */
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
