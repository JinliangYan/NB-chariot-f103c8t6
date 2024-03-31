/****************************************************************************

麦轮小车引脚分配

N20电机

STBY1:PB4    STBY2:PB9

1:PA2,PB6      2:PA3,PB5
3:PA0,PB8      4:PA1,PB7

蓝牙模块：PA9,PA10

超声波模块

TRIG:PB10         ECHO:PB11

RGB彩灯

DI:PB0


************************************************************************************/

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

    while (is_bt_connected() != 1) {
        LED = 1;
        delay_ms(500);
        LED = 0;
        delay_ms(500);
    }
    clean_rebuff();

    while (1) {
        control();
        dlc_control();
        status_handler();
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
