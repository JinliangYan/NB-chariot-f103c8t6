#include "blt.h"
#include "delay.h"
#include "electromagnet.h"
#include "hcsr.h"
#include "led.h"
#include "motor.h"
#include "move.h"
#include "printf.h"
#include "slaver.h"
#include "state.h"
#include "task_runner.h"
#include "ws2812b.h"

void pin_remap(void);
void base_init(void);
void models_init(void);

int
main(void) {
    pin_remap();
    base_init();
    models_init();
    task_runner_init();

    while (1) {
        run();
    }
}

void
base_init(void) {
    chariot.name = "\u5c0f\u9ed1"; //小黑
//    chariot.name = "\u5927\u84dd"; //大蓝
//    chariot.name = "\u5927\u9ed1"; //大黑
    led_init();
    delay_init(); //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    slaver_init();
    hcsr04_init();
    rgb_led_init();
    printf_init();
    motor_init();
}

void
models_init(void) {
    move_init();
    electromagnet_init();
    weapon_init();
    defence_init();
    bt_init();
    state_init();
}

void
pin_remap(void) { /* 释放PB3、PB4、PA15引脚 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    /* 重映射USART1 */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
}
