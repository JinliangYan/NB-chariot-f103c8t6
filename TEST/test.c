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
#include "nrf24l01.h"
#include "stm32f10x.h"
#include "usart.h"
#include "ws2812b.h"
#include "printf.h"

void sysinit();

int debug_test() {
    sysinit();
    delay_init();
    printf_("HELLO\r\n");

    while (1) {
        APP_Joy_Mode();
        dlc_control();
    }
    return 0;
}

void sysinit() {
    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    USART1_Init(9600); //���ڳ�ʼ��Ϊ9600
    NRF24L01_Init();
    //    NRF24L01_Check_detection(); //NRF24L01�ȴ�Ӧ��
    Motor_Init();               //�����ʼ��
    Hcsr04_Init();              //��������ʼ��
    Motion_State(OFF);          //�رյ������ʧ��
    RGB_LED_Init();             //RGB�ʵƳ�ʼ��
    printf_init();
    dlc_init();
    delay_ms(1000);
}
