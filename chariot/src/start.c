#include "blt.h"
#include "delay.h"
#include "electromagnet.h"
#include "hcsr.h"
#include "led.h"
#include "motor.h"
#include "printf.h"
#include "state.h"
#include "task_runner.h"
#include "timer.h"
#include "ws2812b.h"

void pin_remap(void);
void base_init(void);
void models_init(void);

int
main(void) {
    pin_remap();
    base_init();
    models_init();

    while (1) {
        run();
    }
}

void
base_init(void) {
    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    led_init();
    hcsr04_init();  //��������ʼ��
    rgb_led_init(); //RGB�ʵƳ�ʼ��
    printf_init();
    motor_init(); //�����ʼ��
    delay_ms(1000);
}

void
models_init(void) {
    state_init();
    electromagnet_init();
    weapon_init();
    defence_init();
    bt_init();
    tim2_init(72, 1000);
}

void
pin_remap(void) { /* �ͷ�PB3��PB4��PA15���� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    /* ��ӳ��USART1 */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
}
