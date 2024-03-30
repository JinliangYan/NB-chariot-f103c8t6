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

void pin_remap(void);
void base_init(void);

int
main(void) {
    pin_remap();
    base_init();
    dlc_init();

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

void
base_init(void) {
    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    LED_Init();
    Hcsr04_Init();     //��������ʼ��
    Motion_State(OFF); //�رյ������ʧ��
    RGB_LED_Init();    //RGB�ʵƳ�ʼ��
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
