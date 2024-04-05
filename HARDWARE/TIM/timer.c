#include "timer.h"
#include "led.h"

/*
*/

/**
 * \brief 配置生成电机PWM的定时器
 * \param[in] arr: 自动重装载寄存器周期
 * \param[in] psc: 分频值
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim1_init(uint32_t arr, uint32_t psc) {
    TIM_TimeBaseInitTypeDef TIM_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //开启定时器1时钟

    TIM_InitStructure.TIM_Period = arr - 1;                 //自动重装载寄存器周期的值
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //设置预分频值
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //重复计数的值

    TIM_TimeBaseInit(TIM1, &TIM_InitStructure);
    TIM_Cmd(TIM1, ENABLE); //使能计数器
}

/**
 * \brief 配置任务调度的定时器
 * \param[in] arr: 自动重装载寄存器周期
 * \param[in] psc: 分频值
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim2_init(u32 arr, u32 psc) {
    TIM_DeInit(TIM2);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //开启定时器2时钟

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    TIM_InitStructure.TIM_Period = arr - 1;                 //自动重装载寄存器周期的值
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //设置预分频值
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //重复计数的值

    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能计数器中断

    TIM_Cmd(TIM2, ENABLE); //使能计数器
}

/**
 * \brief 配置生成舵机PWM的定时器
 * \param[in] arr: 自动重装载寄存器周期
 * \param[in] psc: 分频值
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim3_init(u32 arr, u32 psc) {
    TIM_DeInit(TIM3);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //开启定时器3时钟

    // NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    // NVIC_Init(&NVIC_InitStructure);

    TIM_InitStructure.TIM_Period = arr - 1;                 //自动重装载寄存器周期的值
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //设置预分频值
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //重复计数的值

    TIM_TimeBaseInit(TIM3, &TIM_InitStructure);

    // TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //使能计数器中断

    TIM_Cmd(TIM3, ENABLE); //使能计数器
}

/**
 * \brief 配置超声波计数定时器
 * \param[in] arr: 自动重装载寄存器周期
 * \param[in] psc: 分频值
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim4_init(u32 arr, u32 psc) {
    TIM_DeInit(TIM4);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //开启定时器4时钟

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    TIM_InitStructure.TIM_Period = arr - 1;                 //自动重装载寄存器周期的值
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //设置预分频值
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //重复计数的值

    TIM_TimeBaseInit(TIM4, &TIM_InitStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //使能计数器中断

    // TIM_Cmd(TIM4, ENABLE); //使能计数器
    TIM_Cmd(TIM4, DISABLE); //使能计数器
}

/*****************************************************************************

//TIM2中断函数
void TIM2_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)      //接收中断标志位
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);        //清除标志位
	}
}

//TIM3中断函数
void TIM3_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)      //接收中断标志位
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);        //清除标志位
	}
}

//TIM4中断函数
void TIM4_IRQHandler(void)                             //TIM4中断函数
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update)!=RESET)      //接收中断标志位
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);        //清除标志位
	}
}

************************************************************************************/
