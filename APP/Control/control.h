/*** 
 * @Author: Jinliang miku.cy@foxmail.com
 * @Date: 2024-03-08 22:22:28
 * @LastEditors: Jinliang miku.cy@foxmail.com
 * @LastEditTime: 2024-03-16 21:38:45
 * @FilePath: \NB-chariot-f103c8t6\APP\Control\control.h
 * @Description: 
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef __CONTROL_H
#define __CONTROL_H
#include "delay.h"
#include "hcsr.h"
#include "math.h"
#include "motor.h"
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "ws2812b.h"

/**
 * \brief       HSVתRGB����ɫ
*/
extern float RGB_R, RGB_G, RGB_B;

/**
 * \brief       ƫ�������ݽ��ջ�����
 * \note        ������!
*/
extern char Pitch_Roll_Buf[20];

void joystick_mode(void);
void gravity_mode(void);

void mode_switch(void);
float get_dis_and_avoid(void);
void gravity_mode(void);
void follow_mode(void);
void rgb_select(void);
void rgb_show(void);
void control(void);

#endif
