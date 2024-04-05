#include "timer.h"
#include "led.h"

/*
*/

/**
 * \brief �������ɵ��PWM�Ķ�ʱ��
 * \param[in] arr: �Զ���װ�ؼĴ�������
 * \param[in] psc: ��Ƶֵ
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim1_init(uint32_t arr, uint32_t psc) {
    TIM_TimeBaseInitTypeDef TIM_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //������ʱ��1ʱ��

    TIM_InitStructure.TIM_Period = arr - 1;                 //�Զ���װ�ؼĴ������ڵ�ֵ
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //����Ԥ��Ƶֵ
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //�ظ�������ֵ

    TIM_TimeBaseInit(TIM1, &TIM_InitStructure);
    TIM_Cmd(TIM1, ENABLE); //ʹ�ܼ�����
}

/**
 * \brief ����������ȵĶ�ʱ��
 * \param[in] arr: �Զ���װ�ؼĴ�������
 * \param[in] psc: ��Ƶֵ
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim2_init(u32 arr, u32 psc) {
    TIM_DeInit(TIM2);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //������ʱ��2ʱ��

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);

    TIM_InitStructure.TIM_Period = arr - 1;                 //�Զ���װ�ؼĴ������ڵ�ֵ
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //����Ԥ��Ƶֵ
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //�ظ�������ֵ

    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //ʹ�ܼ������ж�

    TIM_Cmd(TIM2, ENABLE); //ʹ�ܼ�����
}

/**
 * \brief �������ɶ��PWM�Ķ�ʱ��
 * \param[in] arr: �Զ���װ�ؼĴ�������
 * \param[in] psc: ��Ƶֵ
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim3_init(u32 arr, u32 psc) {
    TIM_DeInit(TIM3);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    // NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //������ʱ��3ʱ��

    // NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    // NVIC_Init(&NVIC_InitStructure);

    TIM_InitStructure.TIM_Period = arr - 1;                 //�Զ���װ�ؼĴ������ڵ�ֵ
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //����Ԥ��Ƶֵ
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //�ظ�������ֵ

    TIM_TimeBaseInit(TIM3, &TIM_InitStructure);

    // TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //ʹ�ܼ������ж�

    TIM_Cmd(TIM3, ENABLE); //ʹ�ܼ�����
}

/**
 * \brief ���ó�����������ʱ��
 * \param[in] arr: �Զ���װ�ؼĴ�������
 * \param[in] psc: ��Ƶֵ
 * \note time_out = arr * psc / 72 * 10^6 us
 */
void
tim4_init(u32 arr, u32 psc) {
    TIM_DeInit(TIM4);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //������ʱ��4ʱ��

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);

    TIM_InitStructure.TIM_Period = arr - 1;                 //�Զ���װ�ؼĴ������ڵ�ֵ
    TIM_InitStructure.TIM_Prescaler = psc - 1;              //����Ԥ��Ƶֵ
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_InitStructure.TIM_RepetitionCounter = 0;            //�ظ�������ֵ

    TIM_TimeBaseInit(TIM4, &TIM_InitStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //ʹ�ܼ������ж�

    // TIM_Cmd(TIM4, ENABLE); //ʹ�ܼ�����
    TIM_Cmd(TIM4, DISABLE); //ʹ�ܼ�����
}

/*****************************************************************************

//TIM2�жϺ���
void TIM2_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET)      //�����жϱ�־λ
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);        //�����־λ
	}
}

//TIM3�жϺ���
void TIM3_IRQHandler(void)                             
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)      //�����жϱ�־λ
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);        //�����־λ
	}
}

//TIM4�жϺ���
void TIM4_IRQHandler(void)                             //TIM4�жϺ���
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update)!=RESET)      //�����жϱ�־λ
	{
		LED=~LED;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);        //�����־λ
	}
}

************************************************************************************/
