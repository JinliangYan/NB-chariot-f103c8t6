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
    pwm2_init(arr, psc);
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

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStruct);
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
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //����JTAG

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_ResetBits(GPIOB, GPIO_Pin_4 | GPIO_Pin_9);
}

/**************************************************
�������ƣ�Motor_Init(void)
�������ܣ����������ų�ʼ��
��ڲ�������
���ز�������
***************************************************/
void
Motor_Init(void) {
    Motor_PWM_Init(500, 72);
    Motor_GPIO_Init();
    STBY_Init();
}

/**************************************************
�������ƣ�forward(u16 speed)
�������ܣ�С��ǰ��
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
forward(u16 speed) {
    TIM_SetCompare1(TIM2, 500 - speed); //R_AIN2:����
    R_AIN2_ON;

    TIM_SetCompare2(TIM2, speed); //R_BIN2:����
    R_BIN2_OFF;

    TIM_SetCompare3(TIM2, speed); //L_AIN2:����
    L_AIN2_OFF;

    TIM_SetCompare4(TIM2, 500 - speed); //L_BIN2:����
    L_BIN2_ON;
}

/**************************************************
�������ƣ�backward(u16 speed)
�������ܣ�С������
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
backward(u16 speed) {
    TIM_SetCompare1(TIM2, speed); //R_AIN2:����
    R_AIN2_OFF;

    TIM_SetCompare2(TIM2, 500 - speed); //R_BIN2:����
    R_BIN2_ON;

    TIM_SetCompare3(TIM2, 500 - speed); //L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM2, speed); //L_BIN2:����
    L_BIN2_OFF;
}

/**************************************************
�������ƣ�Left_Turn(u16 speed)
�������ܣ�С����ת
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
Left_Turn(u16 speed) {
    TIM_SetCompare1(TIM2, 500 - speed); //R_AIN2:����
    R_AIN2_ON;

    TIM_SetCompare2(TIM2, 500 - speed); //R_BIN2:����
    R_BIN2_ON;

    TIM_SetCompare3(TIM2, speed); //L_AIN2:����
    L_AIN2_OFF;

    TIM_SetCompare4(TIM2, speed); //L_BIN2:����
    L_BIN2_OFF;
}

/**************************************************
�������ƣ�Right_Turn(u16 speed)
�������ܣ�С����ת
��ڲ�����speed  0-500
���ز�������
***************************************************/
void
Right_Turn(u16 speed) {
    TIM_SetCompare1(TIM2, speed); //R_AIN2:����
    R_AIN2_OFF;

    TIM_SetCompare2(TIM2, speed); //R_BIN2:����
    R_BIN2_OFF;

    TIM_SetCompare3(TIM2, 500 - speed); //L_AIN2:����
    L_AIN2_ON;

    TIM_SetCompare4(TIM2, 500 - speed); //L_BIN2:����
    L_BIN2_ON;
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
        TIM_SetCompare1(TIM2, speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2, speed); //R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2, speed); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2, speed); //L_BIN2:����
        L_BIN2_OFF;
    } else if (Dir == 1) //����
    {
        TIM_SetCompare1(TIM2, 500 - speed); //R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM2, 500 - speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2, 500 - speed); //L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2, 500 - speed); //L_BIN2:����
        L_BIN2_ON;
    } else if (Dir == 2) //�����ƶ�
    {
        TIM_SetCompare1(TIM2, 500 - speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2, speed); //R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2, speed); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2, 500 - speed); //L_BIN2:����
        L_BIN2_OFF;
    } else if (Dir == 3) //�����ƶ�
    {
        TIM_SetCompare1(TIM2, 500 - speed); //R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM2, speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2, speed); //L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2, 500 - speed); //L_BIN2:����
        L_BIN2_ON;
    } else if (Dir == 4) //�����ƶ�
    {
        TIM_SetCompare1(TIM2, 500 - speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2, 500 - speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2, 500 - speed); //L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2, 500 - speed); //L_BIN2:����
        L_BIN2_OFF;
    } else if (Dir == 5) //�����ƶ�
    {
        TIM_SetCompare1(TIM2, speed); //R_AIN2:����
        R_AIN2_OFF;

        TIM_SetCompare2(TIM2, speed); //R_BIN2:����
        R_BIN2_ON;

        TIM_SetCompare3(TIM2, speed); //L_AIN2:����
        L_AIN2_ON;

        TIM_SetCompare4(TIM2, speed); //L_BIN2:����
        L_BIN2_OFF;
    }
}

/**************************************************
�������ƣ�Motion_State(u16 mode)
�������ܣ�С���رռ���
��ڲ�����mode (ON OFF)
���ز�������
***************************************************/
void
Motion_State(u16 mode) {
    if (mode == ON) {
        L_STBY_ON;
        R_STBY_ON;
        TIM_SetCompare1(TIM2, 500); //R_AIN2:����
        R_AIN2_ON;

        TIM_SetCompare2(TIM2, 0); //R_BIN2:����
        R_BIN2_OFF;

        TIM_SetCompare3(TIM2, 0); //L_AIN2:����
        L_AIN2_OFF;

        TIM_SetCompare4(TIM2, 500); //L_BIN2:����
        L_BIN2_ON;
    } else if (mode == OFF) {
        L_STBY_OFF;
        R_STBY_OFF;
    }
}
