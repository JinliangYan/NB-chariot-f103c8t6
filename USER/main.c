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
#include "usart.h"

uint8_t status = STATUS_ALIVE;
extern bt_received_data_t bt_received_data;

int
main(void) {
    /* 释放PB3、PB4、PA15引脚 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    delay_init(); //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Motor_Init();            //电机初始化
    LED_Init();
    usart1_init_remap(9600); //串口初始化为9600
    Hcsr04_Init();           //超声波初始化
    Motion_State(OFF);       //关闭电机驱动失能
    RGB_LED_Init();          //RGB彩灯初始化
    dlc_init();
    delay_ms(1000);
    while (is_bt_connected() != 1) {
        // 等待手机连接
    }

    clean_rebuff();

    while (1) {
        control();
        dlc_control();
        status = status_check();
        if (status == STATUS_DEAD) {
            status_control(status);
            break;
        }
    }

    // TODO 死亡结算
}
