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
#include "led.h"
#include "sys.h"
#include "usart.h"

uint8_t status = STATUS_ALIVE;
extern bt_received_data_t bt_received_data;

int
main(void) {
    /* �ͷ�PB3��PB4��PA15���� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Motor_Init();            //�����ʼ��
    LED_Init();
    usart1_init_remap(9600); //���ڳ�ʼ��Ϊ9600
    Hcsr04_Init();           //��������ʼ��
    Motion_State(OFF);       //�رյ������ʧ��
    RGB_LED_Init();          //RGB�ʵƳ�ʼ��
    dlc_init();
    delay_ms(1000);
    while (is_bt_connected() != 1) {
        // �ȴ��ֻ�����
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

    // TODO ��������
}
