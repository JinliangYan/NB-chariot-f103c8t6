#ifndef HC05_H
#define HC05_H

#include "printf.h"
#include "delay.h"
#include "stm32f10x.h"

/**
 * \brief   �������ڲ�����
 */
#define BLT_USART_BAUD_RATE 9600

/**
 * \brief   ���ݰ���ʼλ
 */
#define PACKAGE_START_FLAG  '$'

/**
 * \brief   ���ݰ�����λ
 */
#define PACKAGE_END_FLAG    '*'

/**
 * \brief   ATָ��ظ���ʼλ
 */
#define PACKAGE_AT_FLAG     '+'

#define bt_delay_ms         delay_ms
#define BT_STATA_PIN        GPIO_Pin_5
#define BT_SATAT_PORT       GPIOB

/**
 * \brief ���ģ���Ƿ������״̬
 */
#define is_bt_connected()   GPIO_ReadInputDataBit(BT_SATAT_PORT, BT_STATA_PIN)

typedef struct {
    char addr; //�����豸��ַ���ַ�����ʽ������ɨ��ʱ������ʱʹ��
    char name; //�����豸������
} remote_bt_dev_t;

enum {
    HC05_DEFAULT_TIMEOUT = 200,
    HC05_INQUIRY_DEFAULT_TIMEOUT = 10000,
    HC05_PAIRING_DEFAULT_TIMEOUT = 10000,
    HC05_PASSWORD_MAXLEN = 16,
    HC05_PASSWORD_BUFSIZE = HC05_PASSWORD_MAXLEN + 1,
    HC05_NAME_MAXLEN = 32,
    HC05_NAME_BUFSIZE = HC05_NAME_MAXLEN + 1,
    HC05_ADDRESS_MAXLEN = 14,
    HC05_ADDRESS_BUFSIZE = HC05_ADDRESS_MAXLEN + 1,
};

uint8_t bt_init(void);
uint8_t bt_send_atcmd_with_check(char* cmd, uint8_t clean);
uint8_t bt_send_atcmd_with_wait(char* cmd, uint8_t clean, uint32_t delayms); //Debug
uint8_t bt_reset(void);
void bt_send_atcmd(const char* command, const char* arg);
void bt_send_str(char* str);
void bt_send20_packet(char *str);
void bt_clean_rebuff(void);
int get_line(char* line, char* stream, size_t max_size);

#endif /* _HC05_H */
