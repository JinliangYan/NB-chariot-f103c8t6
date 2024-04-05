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
    delay_init(); //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    led_init();
    hcsr04_init();  //超声波初始化
    rgb_led_init(); //RGB彩灯初始化
    printf_init();
    motor_init(); //电机初始化
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
pin_remap(void) { /* 释放PB3、PB4、PA15引脚 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    /* 重映射USART1 */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
}
