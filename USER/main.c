#include "control.h"
#include "delay.h"
#include "dlc.h"
#include "led.h"
#include "sys.h"

void pin_remap(void);
void base_init(void);

int
main(void) {
    pin_remap();
    base_init();
    dlc_init();

    while (1) {
        state_handler();
        control();
        dlc_control();
    }
}

void
base_init(void) {
    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    Hcsr04_Init();  //��������ʼ��
    RGB_LED_Init(); //RGB�ʵƳ�ʼ��
    printf_init();
    Motor_Init(); //�����ʼ��
    delay_ms(1000);
}

void
pin_remap(void) { /* �ͷ�PB3��PB4��PA15���� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    /* ��ӳ��USART1 */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
}
