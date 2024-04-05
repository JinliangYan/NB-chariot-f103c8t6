#include "motor.h"
#include "pwm.h"

/**************************************************
�������ƣ�Motor_PWM_Init(u32 arr,u32 psc)
�������ܣ����PWM�������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void
Motor_PWM_Init(uint32_t arr, uint32_t psc) {
    pwm1_init(arr, psc);
}

/**************************************************
�������ƣ�Motor_GPIO_Init(void)
�������ܣ����GPIO��ʼ������
��ڲ�������
���ز�������
***************************************************/
void
Motor_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**************************************************
�������ƣ�STBY_Init(void)
�������ܣ�STBY��ʼ��
��ڲ�������
���ز�������
***************************************************/
void
STBY_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

/**************************************************
�������ƣ�motor_init(void)
�������ܣ����������ų�ʼ��
��ڲ�������
���ز�������
***************************************************/
void
motor_init(void) {
    Motor_PWM_Init(500, 72);
    Motor_GPIO_Init();
    STBY_Init();
    motor_state(1);
}

/**************************************************
�������ƣ�forward(u16 speed)
�������ܣ�С��ǰ��
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
forward(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, speed); //R_AIN2:����
    R_AIN2_ON;

    TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:����
    R_BIN2_OFF;

    TIM_SetCompare3(TIM1, speed); //L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:����
    L_BIN2_OFF;
}

/**************************************************
�������ƣ�backward(u16 speed)
�������ܣ�С������
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
backward(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:����
    R_AIN2_OFF;

    TIM_SetCompare2(TIM1, speed); //R_BIN2:����
    R_BIN2_ON;

    TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:����
    L_AIN2_OFF;

    TIM_SetCompare4(TIM1, speed); //L_BIN2:����
    L_BIN2_ON;
}

/**************************************************
�������ƣ�motor_turn_left(u16 speed)
�������ܣ�С����ת
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
motor_turn_left(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, speed); //R_AIN2:����
    R_AIN2_ON;

    TIM_SetCompare2(TIM1, speed); //R_BIN2:����
    R_BIN2_ON;

    TIM_SetCompare3(TIM1, speed); //L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM1, speed); //L_BIN2:����
    L_BIN2_ON;
}

/**************************************************
�������ƣ�motor_turn_right(u16 speed)
�������ܣ�С����ת
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
motor_turn_right(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:����
    R_AIN2_OFF;

    TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:����
    R_BIN2_OFF;

    TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:����
    L_AIN2_OFF;

    TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:����
    L_BIN2_OFF;
}

/************************************************************************
�������ƣ�Move(u16 Dir,u16 speed)
�������ܣ�С��ƽ��
��ڲ�����Dir ƽ�Ʒ���(L_Move R_Move L_U_Move L_D_Move R_U_Move L_D_Move)
					���� speed  0-500
���ز�������
*********************************************************&&*************/
void
Move(u16 Dir, u16 speed) {
    if (Dir == 0) //����
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM1, speed); //L_BIN2:����
        L_BIN2_ON;
    } else if (Dir == 1) //����
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM1, speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:����
        L_BIN2_OFF;
    } else if (Dir == 2) //�����ƶ�
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:����
        L_BIN2_ON;
    } else if (Dir == 3) //�����ƶ�
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:����
        L_BIN2_OFF;
    } else if (Dir == 4) //�����ƶ�
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:����
        L_BIN2_ON;
    } else if (Dir == 5) //�����ƶ�
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, speed); //L_BIN2:����
        L_BIN2_ON;
    }
}

/**************************************************
�������ƣ�motor_state(u16 on)
�������ܣ�С���رռ���
��ڲ�����1(ON), 2(OFF)
���ز�������
***************************************************/
void
motor_state(u16 on) {
    if (on == 1) {
        motor_reset();
        L_STBY_ON;
        R_STBY_ON;
    } else if (on == 0) {
        L_STBY_OFF;
        R_STBY_OFF;
    }
}

void
motor_reset(void) {
    TIM_SetCompare1(TIM1, 0); //R_AIN2:����
    R_AIN2_ON;

    TIM_SetCompare2(TIM1, 500); //R_BIN2:����
    R_BIN2_OFF;

    TIM_SetCompare3(TIM1, 0); //L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM1, 500); //L_BIN2:����
    L_BIN2_OFF;
}
