#ifndef __MOTOR_H
#define __MOTOR_H
#include "delay.h"
#include "sys.h"

#define L_AIN2_ON  GPIO_SetBits(GPIOB, GPIO_Pin_13)   //����ߵ�ƽ
#define L_AIN2_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_13) //����͵�ƽ
#define R_AIN2_ON  GPIO_SetBits(GPIOA, GPIO_Pin_12)   //����ߵ�ƽ
#define R_AIN2_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_12) //����͵�ƽ

#define L_BIN2_ON  GPIO_SetBits(GPIOB, GPIO_Pin_14)   //����ߵ�ƽ
#define L_BIN2_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_14) //����͵�ƽ
#define R_BIN2_ON  GPIO_SetBits(GPIOB, GPIO_Pin_15)   //����ߵ�ƽ
#define R_BIN2_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_15) //����͵�ƽ

#define L_STBY_ON  GPIO_SetBits(GPIOB, GPIO_Pin_12)   //����ߵ�ƽ,�������
#define L_STBY_OFF GPIO_ResetBits(GPIOB, GPIO_Pin_12) //����͵�ƽ,���ֹͣ
#define R_STBY_ON  GPIO_SetBits(GPIOA, GPIO_Pin_15)   //����ߵ�ƽ,�������
#define R_STBY_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_15) //����͵�ƽ,���ֹͣ

void Motor_PWM_Init(u32 arr, u32 psc);
void Motor_GPIO_Init(void);
void STBY_Init(void);
void motor_init(void);
void motor_reset(void);

void forward(u16 speed);
void backward(u16 speed);
void motor_turn_left(u16 speed);
void motor_turn_right(u16 speed);
void Move(u16 Dir, u16 speed);
void motor_state(u16 on);

#endif
