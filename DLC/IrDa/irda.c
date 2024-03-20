#include "irda.h"
#include "delay.h"
#include "stm32f10x.h"
#include "printf.h"

#define IRDA_DEBUG

uint8_t isr_cnt; /* ���ڼ�����˶��ٴ��ж� */

uint32_t frame_data = 0; /* һ֡���ݻ��� */
uint8_t frame_cnt = 0;
uint8_t irda_frame_flag = 0; /* һ֡���ݽ�����ɱ�־ */

/**
  * \brief  ����Ƕ�������жϿ�����NVIC
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* ����P[A|B|C|D|E]11Ϊ�ж�Դ */
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
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

//    EXTI_GenerateSWInterrupt(IRDA_EXTI_LINE);
}

/* ��ȡ�ߵ�ƽ��ʱ�� */
uint8_t
get_pulse_time(void) {
    uint8_t time = 0;
    while (IrDa_DATA_IN()) {
        time++;
        delay_us(20); // ��ʱ 20us
        if (time == 250) {
            return time; // ��ʱ���
        }
    }
    return time;
}

/*
 * ֡������4���ֽڣ���һ���ֽ���ң�ص�ID���ڶ����ֽ��ǵ�һ���ֽڵķ���
 * ������������ң�ص������ļ�ֵ�����ĸ��ֽ��ǵ������ֽڵķ���
 * ���ص���irbyte���� ��һλ�����id �ڶ�λ����Ǽ�ֵ
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

    /* �ǵ����־λ */
    irda_frame_flag = 0;
    ir_data[0] = fou_byte;
    ir_data[1] = tir_byte;
    ir_data[2] = sec_byte;
    ir_data[3] = first_byte;
}

// IO ���ж�, �Ӻ������ͷ�����ݹܽ�
void
IRDA_EXTI_IRQHANDLER_FUN(void) {
#ifdef IRDA_DEBUG
    printf_("IRDA\r\n");
#endif

    uint8_t pulse_time = 0;
    uint8_t leader_code_flag = 0; /* �������־λ�������������ʱ����ʾһ֡���ݿ�ʼ */
    uint8_t irda_data = 0;        /* �����ݴ�λ */

    if (EXTI_GetITStatus(IRDA_EXTI_LINE) != RESET) /* ȷ���Ƿ������EXTI Line�ж� */
    {
        while (1) {
            if (IrDa_DATA_IN() == SET) /* ֻ�����ߵ�ƽ��ʱ�� */
            {
                pulse_time = get_pulse_time();

                /* >=5ms ���������ź� �����ָ��Ż���������ʱ��Ҳ��break����while(1)ѭ�� */
                if (pulse_time >= 250) {
                    break; /* ����while(1)ѭ�� */
                }

                if (pulse_time >= 200 && pulse_time < 250) /* ���ǰ��λ 4ms~4.5ms */
                {
                    leader_code_flag = 1;
                } else if (pulse_time >= 10 && pulse_time < 50) /* 0.56ms: 0.2ms~1ms */
                {
                    irda_data = 0;
                } else if (pulse_time >= 50 && pulse_time < 100) /* 1.68ms��1ms~2ms */
                {
                    irda_data = 1;
                } else if (pulse_time >= 100 && pulse_time <= 200) /* 2.1ms��2ms~4ms */
                {                                                  /* �����룬�ڵڶ����жϳ��� */
                    frame_cnt++;                                   /* ����������1 */
                    isr_cnt++;                                     /* ���ж�һ�μ�1 */
                    break;                                         /* ����while(1)ѭ�� */
                }

                if (leader_code_flag == 1) { /* �ڵ�һ���ж������ */
                    frame_data <<= 1;
                    frame_data += irda_data;
                    frame_cnt = 0;
                    isr_cnt = 1;
                }
            }
        }
        irda_frame_flag = 1;                    /* һ֡���ݽ������ */
        EXTI_ClearITPendingBit(IRDA_EXTI_LINE); //����жϱ�־λ
    }
}
