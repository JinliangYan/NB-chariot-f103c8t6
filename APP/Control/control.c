/*
 * \Author: Jinliang miku.cy@foxmail.com
 * \Date: 2024-03-08 22:22:28
 * \LastEditors: Jinliang miku.cy@foxmail.com
 * \LastEditTime: 2024-03-17 11:10:26
 * \FilePath: \NB-chariot-f103c8t6\APP\Control\control.c
 * \Description: 
 * \symbol_custom_string_obkoro1: 
 * \symbol_custom_string_obkoro10001: Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */

#include "control.h"
#include "mode.h"

#define PWM_MIN (-199)
#define PWM_MAX (199)

static void pwm_data_process(int pwm1, int pwm2, int pwm3, int pwm4);
static int Map(int val, int in_min, int in_max, int out_min, int out_max);

u8 LED_Count = 0; //LED灯的个数

/**
 * \brief: 小车行走控制模式, 默认位摇杆控制
*/
control_mode_t control_mode = CONTROL_MODE_JOYSTICK;

/**
 * \brief   彩灯显示模式, 默认关闭彩灯
 */
rgb_mode_t rgb_mode = RGB_MODE_OFF;

/**
 * \brief   小车全局控制函数
*/
void
control(void) {
    if (bt_received_data.receive_data_flag == 1 && bt_received_data.message_type == BT_MESSAGE_MODE_SWITCH) {
        mode_switch();
        bt_received_data.receive_data_flag = 0;
    }
    if (rgb_mode != RGB_MODE_OFF) {
        rgb_show();
    }
    movement_control();
}

/**
 * \brief           Function for implementing mode switch
*/
void
mode_switch(void) {
    switch (control_mode) {
        case CONTROL_MODE_JOYSTICK:
            movement_control();
            break;
            //        case CONTROL_MODE_GRAVITY: gravity_mode(); break;
        case CONTROL_MODE_EVADIBLE:
            avoid();
            break;
        case CONTROL_MODE_FOLLOW:
            follow_mode();
            break;
        case CONTROL_MODE_RGB_MODE_OFF:
            rgb_mode = RGB_MODE_OFF;
            break;
        case CONTROL_MODE_RGB_MODE_BREATHING:
            rgb_mode = RGB_MODE_BREATHING;
            break;
        case CONTROL_MODE_RGB_MODE_RUNNING:
            rgb_mode = RGB_MODE_RUNNING;
            break;
        case CONTROL_MODE_RGB_MODE_FLASHING:
            rgb_mode = RGB_MODE_FLASHING;
            break;
        default:
            // Handle default case if necessary
            break;
    }
}

/**
 * \brief           Function for implementing evadible mode
 */
void
avoid(void) {
    float dis;
    do {
        dis = Hcsr04GetLength();
        motor_turn_left(PWM_MAX);
    } while (dis <= 15);
    motor_reset();
}

/**
 * \brief           Function for implementing follow mode
 */
void
follow_mode(void) {
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
 * \brief           Function to control RGB LEDs
 */
void
rgb_show(void) {
    static int HSV_H = 0;   //HSV的H数值
    static u8 HSV_flag = 0; //颜色转换时所用标志位
    static uint16_t RGB_mode1 = 0, i = 0;
    static uint16_t RGB_mode2 = 0, j = 0;
    if (rgb_mode == RGB_MODE_BREATHING) {
        //RGB彩灯呼吸流水灯效果
        hsv_to_rgb(HSV_H, 100, 100, &RGB_R, &RGB_G, &RGB_B);
        if (HSV_H == 360) {
            HSV_flag = 1;
        } else if (HSV_H == 0) {
            HSV_flag = 0;
        }
        if (HSV_flag == 0) {
            HSV_H += 10;
        } else if (HSV_flag == 1) {
            HSV_H -= 10;
        }
        for (LED_Count = 0; LED_Count < 6; LED_Count++) {
            RGB_LED_Write_24Bits(RGB_R, RGB_G, RGB_B);
        }
        delay_ms(100);
    } else if (rgb_mode == RGB_MODE_RUNNING) {
        if (i % 3 == 0) {
            for (LED_Count = 0; LED_Count < 6; LED_Count++) {
                hsv_to_rgb((RGB_mode1 % 360), 100, 100, &RGB_R, &RGB_G, &RGB_B);
                RGB_LED_Write_24Bits(RGB_R, RGB_G, RGB_B);
                RGB_mode1 += 60;
                if (RGB_mode1 > 360) {
                    RGB_mode1 = 0;
                    i = 0;
                }
            }
        }
        i++;
        delay_ms(100);
    } else if (rgb_mode == RGB_MODE_FLASHING) {
        RGB_Circulation(RGB_mode2);
        if (j % 3 == 0) {
            RGB_mode2++;
        }
        if (RGB_mode2 > 8) {
            j = 0;
            RGB_mode2 = 0;
        }
        j++;
        delay_ms(100);
    }
}

/**
 * \brief           Movement control function
 */
void
movement_control(void) {
    avoid();

    if (bt_received_data.receive_data_flag == 1 && bt_received_data.message_type == BT_MESSAGE_LEFT_JOYSTICK) {
        int Joy_Lx = (bt_received_data.uart_buff[1] - '0') * 10 + (bt_received_data.uart_buff[2] - '0');
        int Joy_Ly = (bt_received_data.uart_buff[4] - '0') * 10 + (bt_received_data.uart_buff[5] - '0');

        int Map_Lx = Map(Joy_Lx, 10, 90, -127, 127);
        int Map_Ly = Map(Joy_Ly, 10, 90, -127, 127);

        int pwm1 = -Map_Ly - Map_Lx;
        int pwm2 = -Map_Ly + Map_Lx;
        int pwm3 = -Map_Ly - Map_Lx;
        int pwm4 = -Map_Ly + Map_Lx;

        pwm1 = Map(pwm1, -127, 127, PWM_MIN, PWM_MAX);
        pwm2 = Map(pwm2, -127, 127, PWM_MIN, PWM_MAX);
        pwm3 = Map(pwm3, -127, 127, PWM_MIN, PWM_MAX);
        pwm4 = Map(pwm4, -127, 127, PWM_MIN, PWM_MAX);

        pwm_data_process(pwm1, pwm2, pwm3, pwm4);

        bt_received_data.receive_data_flag = 0;

        delay_ms(10);
    }

    if (bt_received_data.receive_data_flag == 1 && bt_received_data.message_type == BT_MESSAGE_TURN) {
        if (bt_received_data.uart_buff[0] == 'L') {
            motor_turn_left(PWM_MAX);
        } else if (bt_received_data.uart_buff[0] == 'R') {
            motor_turn_right(PWM_MAX);
        }

        bt_received_data.receive_data_flag = 0;

        delay_ms(10);
    }
}

/**
 * \brief           Gravity mode control function
 */
//void
//gravity_mode(void) {
//    int i, j = 0, Pitch_flag = 0;
//    int APP_Pitch = 0, APP_Roll = 0;
//    int Pitch_symbel = 1, Roll_symbel = 1; //偏航角符号
//    char Pitch_Buf[10], Roll_Buf[10];
//    int Map_pitch, Map_roll; //映射后的偏航角
//    int pwm1, pwm2, pwm3, pwm4;
//    static int Smoothing_Pitch_Buf[5]; //中值滤波数组
//    static int Smoothing_Roll_Buf[5];  //中值滤波数组
//    static int Smoothing_Count = 0;    //中值滤波采样个数
//    int Pitch_temp, Roll_temp;         //选择排序变量
//
//    L_STBY_ON;
//    R_STBY_ON;
//
//    //提取Roll
//    for (i = 1; i < 20; i++) {
//        if (Pitch_Roll_Buf[i] == '.') {
//            break;
//        }
//        Roll_Buf[i - 1] = Pitch_Roll_Buf[i];
//    }
//    //提取Pitch
//    for (i = 0; i < 20; i++) {
//        if (Pitch_Roll_Buf[i] == ',') {
//            Pitch_flag = 1;
//            i++;
//        }
//        if (Pitch_flag == 1) {
//            if (Pitch_Roll_Buf[i] == '.') {
//                j = 0;
//                break;
//            }
//            Pitch_Buf[j] = Pitch_Roll_Buf[i];
//            j++;
//        }
//    }
//    //将Roll字符串转换为整形数据
//    j = 0;
//    for (i = 10; i >= 0; i--) {
//        if (Roll_Buf[i] >= '0' && Roll_Buf[i] <= '9') {
//            APP_Roll += (Roll_Buf[i] - '0') * pow(10, j);
//            j++;
//        }
//        if (Roll_Buf[0] == '-') {
//            Roll_symbel = -1;
//        }
//    }
//    //将Pitch字符串转换为整形数据
//    j = 0;
//    for (i = 10; i >= 0; i--) {
//        if (Pitch_Buf[i] >= '0' && Pitch_Buf[i] <= '9') {
//            APP_Pitch += (Pitch_Buf[i] - '0') * pow(10, j);
//            j++;
//        }
//        if (Pitch_Buf[0] == '-') {
//            Pitch_symbel = -1;
//        }
//    }
//    //得到整形偏航角数据
//    APP_Pitch = Pitch_symbel * APP_Pitch;
//    APP_Roll = Roll_symbel * APP_Roll;
//    //采样五次
//    Smoothing_Pitch_Buf[Smoothing_Count] = APP_Pitch;
//    Smoothing_Roll_Buf[Smoothing_Count] = APP_Roll;
//    Smoothing_Count++;
//    //选择排序
//    if (Smoothing_Count == 5) {
//        Smoothing_Count = 0;
//
//        for (j = 0; j < 5 - 1; j++) {
//            for (i = 0; i < 5 - j; i++) {
//                if (Smoothing_Pitch_Buf[i] > Smoothing_Pitch_Buf[i + 1]) {
//                    Pitch_temp = Smoothing_Pitch_Buf[i];
//                    Smoothing_Pitch_Buf[i] = Smoothing_Pitch_Buf[i + 1];
//                    Smoothing_Pitch_Buf[i + 1] = Pitch_temp;
//                }
//                if (Smoothing_Roll_Buf[i] > Smoothing_Roll_Buf[i + 1]) {
//                    Roll_temp = Smoothing_Roll_Buf[i];
//                    Smoothing_Roll_Buf[i] = Smoothing_Roll_Buf[i + 1];
//                    Smoothing_Roll_Buf[i + 1] = Roll_temp;
//                }
//            }
//        }
//        //中值滤波
//        APP_Pitch = Smoothing_Pitch_Buf[2];
//        APP_Roll = Smoothing_Roll_Buf[2];
//
//        Map_pitch = Map(APP_Pitch, -90, 90, -127, 127);
//        Map_roll = Map(APP_Roll, -90, 90, -127, 127);
//
//        pwm1 = -Map_pitch + Map_roll;
//        pwm2 = -Map_pitch - Map_roll;
//        pwm3 = -Map_pitch + Map_roll;
//        pwm4 = -Map_pitch - Map_roll;
//
//        pwm1 = Map(pwm1, -127, 127, -PWM_MAX, PWM_MAX);
//        pwm2 = Map(pwm2, -127, 127, -PWM_MAX, PWM_MAX);
//        pwm3 = Map(pwm3, -127, 127, -PWM_MAX, PWM_MAX);
//        pwm4 = Map(pwm4, -127, 127, -PWM_MAX, PWM_MAX);
//
//        pwm_data_process(pwm1, pwm2, pwm3, pwm4);
//
//        memset(Smoothing_Pitch_Buf, 0, sizeof(Smoothing_Pitch_Buf));
//        memset(Smoothing_Roll_Buf, 0, sizeof(Smoothing_Roll_Buf));
//        delay_ms(1);
//    }
//
//    memset(Roll_Buf, 0, 10);
//    memset(Pitch_Buf, 0, 10);
//
//    delay_ms(1);
//}

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

    if (pwm1 > PWM_MAX) {
        pwm1 = PWM_MAX;
    }
    if (pwm2 > PWM_MAX) {
        pwm2 = PWM_MAX;
    }
    if (pwm3 > PWM_MAX) {
        pwm3 = PWM_MAX;
    }
    if (pwm4 > PWM_MAX) {
        pwm4 = PWM_MAX;
    }

    if (pwm1 < PWM_MIN) {
        pwm1 = PWM_MIN;
    }
    if (pwm2 < PWM_MIN) {
        pwm2 = PWM_MIN;
    }
    if (pwm3 < PWM_MIN) {
        pwm3 = PWM_MIN;
    }
    if (pwm4 < PWM_MIN) {
        pwm4 = PWM_MIN;
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
