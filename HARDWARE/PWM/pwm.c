/*
 * @Author: Jinliang miku.cy@foxmail.com
 * @Date: 2024-03-08 22:22:26
 * @LastEditors: Jinliang miku.cy@foxmail.com
 * @LastEditTime: 2024-03-17 15:10:58
 * @FilePath: \NB-chariot-f103c8t6\HARDWARE\PWM\pwm.c
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

/**************************************************
�������ƣ�PWM1_Init(u32 arr,u32 psc)
�������ܣ�PWM1�������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
// void
// PWM1_Init(u32 arr, u32 psc) {
//     GPIO_InitTypeDef GPIO_InitStructure;
//     TIM_TimeBaseInitTypeDef TIM_InitStructure;
//     TIM_OCInitTypeDef TIM_OCInitStructure;

//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //������ʱ��1ʱ��

//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
//     GPIO_Init(GPIOA, &GPIO_InitStructure);            //��ʼ��GPIOA

//     TIM_InitStructure.TIM_Period = arr;                     //�Զ���װ�ؼĴ������ڵ�ֵ
//     TIM_InitStructure.TIM_Prescaler = psc;                  //����Ԥ��Ƶֵ
//     TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
//     TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
//     TIM_InitStructure.TIM_RepetitionCounter = 0;            //�ظ�������ֵ

//     TIM_TimeBaseInit(TIM1, &TIM_InitStructure);
//     TIM_Cmd(TIM1, ENABLE); //ʹ�ܼ�����

//     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
//     TIM_OC1Init(TIM1, &TIM_OCInitStructure);                      //��ʼ������TIM1 OC1
//     TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);             //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���

//     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
//     TIM_OC2Init(TIM1, &TIM_OCInitStructure);                      //��ʼ������TIM1 OC2
//     TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);             //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���

//     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
//     TIM_OC3Init(TIM1, &TIM_OCInitStructure);                      //��ʼ������TIM1 OC3
//     TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);             //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���

//     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
//     TIM_OC4Init(TIM1, &TIM_OCInitStructure);                      //��ʼ������TIM1 OC4
//     TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);             //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���

//     TIM_CtrlPWMOutputs(TIM1, ENABLE);
// }

/**************************************************
�������ƣ�PWM2_Init(u32 arr,u32 psc)
�������ܣ�PWM2�������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void
pwm2_init(u32 arr, u32 psc) {
    tim2_init(arr, psc);

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //��ʼ��GPIOA

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);                      //��ʼ������TIM2 OC1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);             //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      //��ʼ������TIM2 OC2
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);             //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);                      //��ʼ������TIM2 OC3
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);             //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);                      //��ʼ������TIM2 OC4
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);             //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
}

void
pwm3_init(u32 arr, u32 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    tim3_init(arr, psc);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //��ʼ��GPIOA

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    // GPIO_Init(GPIOC, &GPIO_InitStructure);            //��ʼ��GPIOB

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    // TIM_OC3Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC3
    // TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    // TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC4
    // TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
}

/**************************************************
�������ƣ�PWM3_Init(u32 arr,u32 psc)
�������ܣ�PWM3�������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void
pwm3_init_remap(u32 arr, u32 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    tim3_init(arr, psc);

    /* ʹ��AFIOʱ�ӡ���ӳ�����ʹ��AFIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* ѡ����ȫ��ӳ�� */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);            //��ʼ��GPIOA

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    // GPIO_Init(GPIOC, &GPIO_InitStructure);            //��ʼ��GPIOB

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    // TIM_OC3Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC3
    // TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
    // TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //��ʼ������TIM3 OC4
    // TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);             //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
}

/**************************************************
�������ƣ�PWM4_Init(u32 arr,u32 psc)
�������ܣ�PWM4�������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void
pwm4_init(u32 arr, u32 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    tim4_init(arr, psc);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //��ʼ��GPIOB

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);                      //��ʼ������TIM4 OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);             //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ը�
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);                      //��ʼ������TIM4 OC2
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);             //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ը�
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);                      //��ʼ������TIM4 OC3
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);             //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������:TIM����Ƚϼ��Ը�
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);                      //��ʼ������TIM4 OC4
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);             //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
}