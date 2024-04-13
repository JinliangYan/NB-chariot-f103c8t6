#ifndef HC05_H
#define HC05_H

#include "printf.h"
#include "delay.h"
#include "stm32f10x.h"

/**
 * \brief   蓝牙串口波特率
 */
#define BLT_USART_BAUD_RATE 9600

/**
 * \brief   数据包起始位
 */
#define PACKAGE_START_FLAG  '$'

/**
 * \brief   数据包结束位
 */
#define PACKAGE_END_FLAG    '*'

/**
 * \brief   AT指令回复起始位
 */
#define PACKAGE_AT_FLAG     '+'

#define bt_delay_ms         delay_ms
#define BT_STATA_PIN        GPIO_Pin_5
#define BT_SATAT_PORT       GPIOB

/**
 * \brief 检查模块是否处于配对状态
 */
#define is_bt_connected()   GPIO_ReadInputDataBit(BT_SATAT_PORT, BT_STATA_PIN)

typedef struct {
    char addr; //蓝牙设备地址，字符串形式，方便扫描时和连接时使用
    char name; //蓝牙设备的名字
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
