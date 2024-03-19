#include "irda.h"
#include "delay.h"
#include "stm32f10x.h"
uint32_t frame_data = 0; /* һ֡���ݻ��� */
uint8_t frame_cnt = 0;
uint8_t frame_flag = 0; /* һ֡���ݽ�����ɱ�־ */

void
irda_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /* config the extiline clock and AFIO clock */
    RCC_APB2PeriphClockCmd(IRDA_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

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
}

/* ��ȡ�ߵ�ƽ��ʱ�� */
uint8_t
get_pulse_time(void) {
    uint8_t time = 0;
    while (IrDa_DATA_IN()) {
        time++;
        delay_ms(2); // ��ʱ 20us
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
uint8_t
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
    frame_flag = 0;
    ir_data[0] = fou_byte;
    ir_data[1] = tir_byte;
    ir_data[2] = sec_byte;
    ir_data[3] = first_byte;
    return 1;
}
