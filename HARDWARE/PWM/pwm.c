#include "pwm.h"
#include "delay.h"
#include "led.h"
#include "timer.h"

/**************************************************
函数名称：PWM1_Init(u32 arr,u32 psc)
函数功能：PWM1输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void
pwm1_init(u32 arr, u32 psc) {
    tim1_init(arr, psc);

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //初始化GPIOA

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);                      //初始化外设TIM1 OC1
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);             //使能TIM1在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);                      //初始化外设TIM1 OC2
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);             //使能TIM1在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);                      //初始化外设TIM1 OC3
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);             //使能TIM1在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);                      //初始化外设TIM1 OC4
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);             //使能TIM1在CCR2上的预装载寄存器

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/**************************************************
函数名称：PWM2_Init(u32 arr,u32 psc)
函数功能：PWM2输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void
pwm2_init(u32 arr, u32 psc) {
    tim2_init(arr, psc);

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //初始化GPIOA

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);                      //初始化外设TIM2 OC1
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);             //使能TIM2在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      //初始化外设TIM2 OC2
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);             //使能TIM2在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);                      //初始化外设TIM2 OC3
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);             //使能TIM2在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);                      //初始化外设TIM2 OC4
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);             //使能TIM2在CCR2上的预装载寄存器
}

void
pwm3_init(u32 arr, u32 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    tim3_init(arr, psc);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //初始化GPIOA

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    // GPIO_Init(GPIOC, &GPIO_InitStructure);            //初始化GPIOB

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    // TIM_OC3Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC3
    // TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    // TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC4
    // TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器
}

/**************************************************
函数名称：PWM3_Init(u32 arr,u32 psc)
函数功能：PWM3输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void
pwm3_init_remap(u32 arr, u32 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    tim3_init(arr, psc);

    /* 使能AFIO时钟。重映射必须使能AFIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* 选择完全重映射 */
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);            //初始化GPIOA

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    // GPIO_Init(GPIOC, &GPIO_InitStructure);            //初始化GPIOB

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    // TIM_OC3Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC3
    // TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器

    // TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    // TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    // TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    // TIM_OC4Init(TIM3, &TIM_OCInitStructure);                      //初始化外设TIM3 OC4
    // TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);             //使能TIM3在CCR2上的预装载寄存器
}

/**************************************************
函数名称：PWM4_Init(u32 arr,u32 psc)
函数功能：PWM4输出函数
入口参数：arr 重装载值    psc   预分频系数
返回参数：无
***************************************************/
void
pwm4_init(u32 arr, u32 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    tim4_init(arr, psc);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //初始化GPIOB

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);                      //初始化外设TIM4 OC1
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);             //使能TIM4在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性高
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);                      //初始化外设TIM4 OC2
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);             //使能TIM4在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);                      //初始化外设TIM4 OC3
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);             //使能TIM4在CCR2上的预装载寄存器

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性:TIM输出比较极性高
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);                      //初始化外设TIM4 OC4
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);             //使能TIM4在CCR2上的预装载寄存器
}