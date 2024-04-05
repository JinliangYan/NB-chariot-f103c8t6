#include "motor.h"
#include "pwm.h"

/**************************************************
函数名称：Motor_PWM_Init(u32 arr,u32 psc)
函数功能：电机PWM输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void
Motor_PWM_Init(uint32_t arr, uint32_t psc) {
    pwm1_init(arr, psc);
}

/**************************************************
函数名称：Motor_GPIO_Init(void)
函数功能：电机GPIO初始化函数
入口参数：无
返回参数：无
***************************************************/
void
Motor_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**************************************************
函数名称：STBY_Init(void)
函数功能：STBY初始化
入口参数：无
返回参数：无
***************************************************/
void
STBY_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}

/**************************************************
函数名称：motor_init(void)
函数功能：电机相关引脚初始化
入口参数：无
返回参数：无
***************************************************/
void
motor_init(void) {
    Motor_PWM_Init(500, 72);
    Motor_GPIO_Init();
    STBY_Init();
    motor_state(1);
}

/**************************************************
函数名称：forward(u16 speed)
函数功能：小车前进
入口参数：speed  0-500
返回参数：无
***************************************************/
void
forward(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, speed); //R_AIN2:右下
    R_AIN2_ON;

    TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:左下
    R_BIN2_OFF;

    TIM_SetCompare3(TIM1, speed); //L_AIN2:右上
    L_AIN2_ON;

    TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:左上
    L_BIN2_OFF;
}

/**************************************************
函数名称：backward(u16 speed)
函数功能：小车后退
入口参数：speed  0-500
返回参数：无
***************************************************/
void
backward(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:右下
    R_AIN2_OFF;

    TIM_SetCompare2(TIM1, speed); //R_BIN2:左下
    R_BIN2_ON;

    TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:右上
    L_AIN2_OFF;

    TIM_SetCompare4(TIM1, speed); //L_BIN2:左上
    L_BIN2_ON;
}

/**************************************************
函数名称：motor_turn_left(u16 speed)
函数功能：小车左转
入口参数：speed  0-500
返回参数：无
***************************************************/
void
motor_turn_left(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, speed); //R_AIN2:右下
    R_AIN2_ON;

    TIM_SetCompare2(TIM1, speed); //R_BIN2:左下
    R_BIN2_ON;

    TIM_SetCompare3(TIM1, speed); //L_AIN2:右上
    L_AIN2_ON;

    TIM_SetCompare4(TIM1, speed); //L_BIN2:左上
    L_BIN2_ON;
}

/**************************************************
函数名称：motor_turn_right(u16 speed)
函数功能：小车右转
入口参数：speed  0-500
返回参数：无
***************************************************/
void
motor_turn_right(u16 speed) {
    L_STBY_ON;
    R_STBY_ON;

    TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:右下
    R_AIN2_OFF;

    TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:左下
    R_BIN2_OFF;

    TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:右上
    L_AIN2_OFF;

    TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:左上
    L_BIN2_OFF;
}

/************************************************************************
函数名称：Move(u16 Dir,u16 speed)
函数功能：小车平移
入口参数：Dir 平移方向(L_Move R_Move L_U_Move L_D_Move R_U_Move L_D_Move)
					方向 speed  0-500
返回参数：无
*********************************************************&&*************/
void
Move(u16 Dir, u16 speed) {
    if (Dir == 0) //左移
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:左下
        R_BIN2_OFF;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:右上
        L_AIN2_ON;

        TIM_SetCompare4(TIM1, speed); //L_BIN2:左上
        L_BIN2_ON;
    } else if (Dir == 1) //右移
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:右下
        R_AIN2_ON;

        TIM_SetCompare2(TIM1, speed); //R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:左上
        L_BIN2_OFF;
    } else if (Dir == 2) //左上移动
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:左下
        R_BIN2_OFF;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:右上
        L_AIN2_ON;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:左上
        L_BIN2_ON;
    } else if (Dir == 3) //右上移动
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:右下
        R_AIN2_ON;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:左上
        L_BIN2_OFF;
    } else if (Dir == 4) //左下移动
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, speed); //R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, speed); //R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, 500 - speed); //L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, 500 - speed); //L_BIN2:左上
        L_BIN2_ON;
    } else if (Dir == 5) //右下移动
    {
        L_STBY_ON;
        R_STBY_ON;

        TIM_SetCompare1(TIM1, 500 - speed); //R_AIN2:右下
        R_AIN2_OFF;

        TIM_SetCompare2(TIM1, 500 - speed); //R_BIN2:左下
        R_BIN2_ON;

        TIM_SetCompare3(TIM1, speed); //L_AIN2:右上
        L_AIN2_OFF;

        TIM_SetCompare4(TIM1, speed); //L_BIN2:左上
        L_BIN2_ON;
    }
}

/**************************************************
函数名称：motor_state(u16 on)
函数功能：小车关闭及打开
入口参数：1(ON), 2(OFF)
返回参数：无
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
    TIM_SetCompare1(TIM1, 0); //R_AIN2:右下
    R_AIN2_ON;

    TIM_SetCompare2(TIM1, 500); //R_BIN2:左下
    R_BIN2_OFF;

    TIM_SetCompare3(TIM1, 0); //L_AIN2:右上
    L_AIN2_ON;

    TIM_SetCompare4(TIM1, 500); //L_BIN2:左上
    L_BIN2_OFF;
}
