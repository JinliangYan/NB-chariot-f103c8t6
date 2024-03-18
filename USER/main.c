/****************************************************************************

����С�����ŷ���

N20���

STBY1:PB4    STBY2:PB9

1:PA2,PB6      2:PA3,PB5
3:PA0,PB8      4:PA1,PB7

����ģ�飺PA9,PA10

������ģ��

TRIG:PB10         ECHO:PB11

RGB�ʵ�

DI:PB0


************************************************************************************/

#include "control.h"
#include "delay.h"
#include "dlc.h"
#include "stdio.h"
#include "sys.h"
#include "led.h"
#include "usart.h"

int
main(void) {
    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    usart1_init_remap(9600); //���ڳ�ʼ��Ϊ9600
    //    NRF24L01_Check_detection(); //NRF24L01�ȴ�Ӧ��
    Motor_Init();      //�����ʼ��
    Hcsr04_Init();     //��������ʼ��
    Motion_State(OFF); //�رյ������ʧ��
    RGB_LED_Init();    //RGB�ʵƳ�ʼ��
    dlc_init();
    delay_ms(1000);
    while (1) {
        control();
        dlc_control();
    }
}
