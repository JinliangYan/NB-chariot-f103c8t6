/**
* \file            move.c
* \date            4/5/2024
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

#include <string.h>
#include "delay.h"
#include "hcsr.h"
#include "mode.h"
#include "motor.h"
#include "move.h"
#include "slaver.h"
#include "state.h"
#include "usart.h"

static void pwm_data_process(int pwm1, int pwm2, int pwm3, int pwm4);
static int Map(int val, int in_min, int in_max, int out_min, int out_max);
static void avoid(void);
static void follow(void);
static void clear_attacker_info(void);

static attacker_t attacker;
static uint16_t speed_max;

/**
 * \brief 所有类型结构体数组, 以防具类型为index
 */
const move_model_t move_models[10] = {
    [MOVE_MODEL_TYPE_LIGHTWEIGHT] = {
        .type = MOVE_MODEL_TYPE_LIGHTWEIGHT,
        .hp = HP_S,
        .speed = 499,
        .name = "MOVE_MODEL_TYPE_LIGHTWEIGHT",
    },
    [MOVE_MODEL_TYPE_HEAVYWEIGHT] = {
        .type = MOVE_MODEL_TYPE_HEAVYWEIGHT,
        .hp = HP_L,
        .speed = 349,
        .name = "MOVE_MODEL_TYPE_HEAVYWEIGHT",
    },
    // TODO 补充其他移动模块类型
};

move_model_t move_model;

/**
 * \brief 移动模块初始化
 */
void
move_init(void) {
    move_model_type_t move_model_type = MOVE_MODEL_TYPE_BEGIN;

    /* 1. 扫描，确定类型 */
    for (move_model_type_t i = move_model_type; i < MOVE_MODEL_TYPE_END; ++i) {
        if (slaver_model_addr_confirm("move", i)) {
            move_model_type = i;
            break;
        }
    }

    /* 设置默认值 */
    if (move_model_type == MOVE_MODEL_TYPE_BEGIN) {
        move_model_type = MOVE_MODEL_TYPE_LIGHTWEIGHT;
    }

    move_model = move_models[move_model_type];

    /* 2. 根据总重确定速度 */
    speed_max = chariot.speed_with_weight;
}

/**
 * \brief           Movement control function
 */
void
move_control(void) {
    speed_max = chariot.speed_with_weight;

    /* 从副板获得攻击者ID，伤害，技能 */
    if (slaver_received_data.receive_data_flag == 1 && slaver_received_data.message_type == SLAVER_MESSAGE_INFO) {

        if (strstr((char*)slaver_received_data.uart_buff, "attacked")
            && strstr((char*)slaver_received_data.uart_buff, "move")) {

            sscanf((char*)slaver_received_data.uart_buff,
                  "attacked-i%d-s%d-p%d",
                  &attacker.id, &attacker.skill, &attacker.power);
        }
    }
    /* 扣血 */
    if (attacker.id != 0 && attacker.id != CHARIOT_ID) {
            move_model.hp -= attacker.power;
            if (move_model.hp < 0) {
                move_model.hp = 0;
            }
            state_update_info(MODEL_MOVE, ATTRIBUTE_HP, move_model.hp);
            clear_attacker_info();
    }

    avoid();
    if (bt_received_data.receive_data_flag == 1 && bt_received_data.message_type == BT_MESSAGE_LEFT_JOYSTICK) {
        int Joy_Lx = (bt_received_data.uart_buff[1] - '0') * 10 + (bt_received_data.uart_buff[2] - '0');
        int Joy_Ly = (bt_received_data.uart_buff[4] - '0') * 10 + (bt_received_data.uart_buff[5] - '0');

        int Map_Lx = Map(Joy_Lx, 10, 90, -127, 127);
        int Map_Ly = Map(Joy_Ly, 10, 90, -127, 127);

        int pwm1 = Map_Ly + Map_Lx;
        int pwm2 = Map_Ly - Map_Lx;
        int pwm3 = Map_Ly + Map_Lx;
        int pwm4 = Map_Ly - Map_Lx;
//        int pwm1 = -Map_Ly - Map_Lx;
//        int pwm2 = -Map_Ly + Map_Lx;
//        int pwm3 = -Map_Ly - Map_Lx;
//        int pwm4 = -Map_Ly + Map_Lx;

        pwm1 = Map(pwm1, -127, 127, -speed_max, speed_max);
        pwm2 = Map(pwm2, -127, 127, -speed_max, speed_max);
        pwm3 = Map(pwm3, -127, 127, -speed_max, speed_max);
        pwm4 = Map(pwm4, -127, 127, -speed_max, speed_max);

        pwm_data_process(pwm1, pwm2, pwm3, pwm4);

        bt_received_data.receive_data_flag = 0;
        delay_ms(50);
    }

    if (bt_received_data.receive_data_flag == 1 && bt_received_data.message_type == BT_MESSAGE_TURN) {
        if (bt_received_data.uart_buff[0] == 'L') {
//            motor_turn_right(200);
            motor_turn_left(200);
        } else if (bt_received_data.uart_buff[0] == 'R') {
//            motor_turn_right(200);
            motor_turn_right(200);
        }

        bt_received_data.receive_data_flag = 0;
        delay_ms(50);
    }
}

/**
 * \brief           Function for implementing evadible mode
 */
static void
avoid(void) {
    float dis;
    do {
        dis = Hcsr04GetLength();
        motor_turn_left(speed_max);
    } while (dis <= 15);
    motor_reset();
}

/**
 * \brief           Function for implementing follow mode
 */
static void
follow(void) {
    float Dis;
    Dis = Hcsr04GetLength();
    if (Dis <= 10) {
        backward(200);
    } else if (Dis <= 30 && Dis >= 20) {
        forward(200);
    } else {
        motor_state(1);
    }
}

/**
 * \brief           Process PWM data and control motor movement
 *
 * \param[in]       pwm1: PWM value for motor 1
 * \param[in]       pwm2: PWM value for motor 2
 * \param[in]       pwm3: PWM value for motor 3
 * \param[in]       pwm4: PWM value for motor 4
 */
static void
pwm_data_process(int pwm1, int pwm2, int pwm3, int pwm4) {
    if (pwm1 < 20 && pwm1 > -20) {
        pwm1 = 0;
    }
    if (pwm2 < 20 && pwm2 > -20) {
        pwm2 = 0;
    }
    if (pwm3 < 20 && pwm3 > -20) {
        pwm3 = 0;
    }
    if (pwm4 < 20 && pwm4 > -20) {
        pwm4 = 0;
    }

    if (pwm1 > speed_max) {
        pwm1 = speed_max;
    }
    if (pwm2 > speed_max) {
        pwm2 = speed_max;
    }
    if (pwm3 > speed_max) {
        pwm3 = speed_max;
    }
    if (pwm4 > speed_max) {
        pwm4 = speed_max;
    }

    if (pwm1 < -speed_max) {
        pwm1 = -speed_max;
    }
    if (pwm2 < -speed_max) {
        pwm2 = -speed_max;
    }
    if (pwm3 < -speed_max) {
        pwm3 = -speed_max;
    }
    if (pwm4 < -speed_max) {
        pwm4 = -speed_max;
    }

    if (pwm1 >= 0) {
        TIM_SetCompare4(TIM1, pwm1); //L_BIN2:左上轮
        L_BIN2_ON;

    } else if (pwm1 < 0) {
        pwm1 = abs(pwm1);
        TIM_SetCompare4(TIM1, 500 - pwm1); //L_BIN2:左上轮
        L_BIN2_OFF;
    }

    if (pwm2 >= 0) {
        TIM_SetCompare3(TIM1, 500 - pwm2); //L_AIN2:右上轮
        L_AIN2_OFF;
    } else if (pwm2 < 0) {
        pwm2 = abs(pwm2);
        TIM_SetCompare3(TIM1, pwm2); //L_AIN2:右上轮
        L_AIN2_ON;
    }

    if (pwm3 >= 0) {
        TIM_SetCompare1(TIM1, 500 - pwm3); //R_AIN2:右下轮
        R_AIN2_OFF;
    } else if (pwm3 < 0) {
        pwm3 = abs(pwm3);
        TIM_SetCompare1(TIM1, pwm3); //R_AIN2:右下轮
        R_AIN2_ON;
    }

    if (pwm4 >= 0) {
        TIM_SetCompare2(TIM1, pwm4); //R_BIN2:左下轮
        R_BIN2_ON;
    } else if (pwm4 < 0) {
        pwm4 = abs(pwm4);
        TIM_SetCompare2(TIM1, 500 - pwm4); //R_BIN2:左下轮
        R_BIN2_OFF;
    }
}

/**
 * \brief           Map a value from one range to another
 *
 * \param[in]       val: Input value to map
 * \param[in]       in_min: Minimum value of the input range
 * \param[in]       in_max: Maximum value of the input range
 * \param[in]       out_min: Minimum value of the output range
 * \param[in]       out_max: Maximum value of the output range
 * \return          Mapped value within the output range
 */
static int
Map(int val, int in_min, int in_max, int out_min, int out_max) {
    return (int)(val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void
clear_attacker_info(void) {
    attacker.power = 0;
    attacker.id = 0;
    attacker.skill = 0;
}
