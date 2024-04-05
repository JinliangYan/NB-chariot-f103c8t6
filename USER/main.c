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
    delay_init(); //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    Hcsr04_Init();  //超声波初始化
    RGB_LED_Init(); //RGB彩灯初始化
    printf_init();
    Motor_Init(); //电机初始化
    delay_ms(1000);
}

void
pin_remap(void) { /* 释放PB3、PB4、PA15引脚 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    /* 重映射USART1 */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
}
