#include "irda.h"
#include "delay.h"
#include "stm32f10x.h"
#include "printf.h"

#define IRDA_DEBUG

uint8_t isr_cnt; /* 用于计算进了多少次中断 */

uint32_t frame_data = 0; /* 一帧数据缓存 */
uint8_t frame_cnt = 0;
uint8_t irda_frame_flag = 0; /* 一帧数据接收完成标志 */

/**
  * \brief  配置嵌套向量中断控制器NVIC
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* 配置P[A|B|C|D|E]11为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = IRDA_EXTI_IRQN;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void
irda_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /* config the extiline clock and AFIO clock */
    RCC_APB2PeriphClockCmd(IRDA_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

    /* config the NVIC */
    NVIC_Configuration();

    /* EXTI line gpio config */
    GPIO_InitStructure.GPIO_Pin = IRDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(IRDA_GPIO_PORT, &GPIO_InitStructure);

    /* EXTI line mode config */
    GPIO_EXTILineConfig(IRDA_GPIO_PORT_SOURCE, IRDA_GPIO_PIN_SOURCE);
    EXTI_InitStructure.EXTI_Line = IRDA_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

//    EXTI_GenerateSWInterrupt(IRDA_EXTI_LINE);
}

/* 获取高电平的时间 */
uint8_t
get_pulse_time(void) {
    uint8_t time = 0;
    while (IrDa_DATA_IN()) {
        time++;
        delay_us(20); // 延时 20us
        if (time == 250) {
            return time; // 超时溢出
        }
    }
    return time;
}

/*
 * 帧数据有4个字节，第一个字节是遥控的ID，第二个字节是第一个字节的反码
 * 第三个数据是遥控的真正的键值，第四个字节是第三个字节的反码
 * 返回的是irbyte数组 第一位存的是id 第二位存的是键值
 */
void
irda_process(uint8_t ir_data[]) {
    uint32_t data = 0, i;
    uint8_t first_byte, sec_byte, tir_byte, fou_byte;
    for (i = 0; i < 32; i++) {
        data += frame_data & 0x0001;
        data <<= 1;
        frame_data >>= 1;
    }
    data >>= 1;
    first_byte = data >> 24;
    sec_byte = (data >> 16) & 0xff;
    tir_byte = data >> 8;
    fou_byte = data;

    /* 记得清标志位 */
    irda_frame_flag = 0;
    ir_data[0] = fou_byte;
    ir_data[1] = tir_byte;
    ir_data[2] = sec_byte;
    ir_data[3] = first_byte;
}

// IO 线中断, 接红外接收头的数据管脚
void
IRDA_EXTI_IRQHANDLER_FUN(void) {
#ifdef IRDA_DEBUG
    printf_("IRDA\r\n");
#endif

    uint8_t pulse_time = 0;
    uint8_t leader_code_flag = 0; /* 引导码标志位，当引导码出现时，表示一帧数据开始 */
    uint8_t irda_data = 0;        /* 数据暂存位 */

    if (EXTI_GetITStatus(IRDA_EXTI_LINE) != RESET) /* 确保是否产生了EXTI Line中断 */
    {
        while (1) {
            if (IrDa_DATA_IN() == SET) /* 只测量高电平的时间 */
            {
                pulse_time = get_pulse_time();

                /* >=5ms 不是有用信号 当出现干扰或者连发码时，也会break跳出while(1)循环 */
                if (pulse_time >= 250) {
                    break; /* 跳出while(1)循环 */
                }

                if (pulse_time >= 200 && pulse_time < 250) /* 获得前导位 4ms~4.5ms */
                {
                    leader_code_flag = 1;
                } else if (pulse_time >= 10 && pulse_time < 50) /* 0.56ms: 0.2ms~1ms */
                {
                    irda_data = 0;
                } else if (pulse_time >= 50 && pulse_time < 100) /* 1.68ms：1ms~2ms */
                {
                    irda_data = 1;
                } else if (pulse_time >= 100 && pulse_time <= 200) /* 2.1ms：2ms~4ms */
                {                                                  /* 连发码，在第二次中断出现 */
                    frame_cnt++;                                   /* 按键次数加1 */
                    isr_cnt++;                                     /* 进中断一次加1 */
                    break;                                         /* 跳出while(1)循环 */
                }

                if (leader_code_flag == 1) { /* 在第一次中断中完成 */
                    frame_data <<= 1;
                    frame_data += irda_data;
                    frame_cnt = 0;
                    isr_cnt = 1;
                }
            }
        }
        irda_frame_flag = 1;                    /* 一帧数据接收完成 */
        EXTI_ClearITPendingBit(IRDA_EXTI_LINE); //清除中断标志位
    }
}
