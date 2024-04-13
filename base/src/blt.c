#include "blt.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

static char* get_rebuff(uint16_t* len);


/**
  * @brief  初始化GPIO及检测HC05模块
  * @param  无
  * @retval HC05状态，0 正常，非0异常
  */
uint8_t
bt_init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = BT_STATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BT_SATAT_PORT, &GPIO_InitStructure);
    usart1_init_remap(BLT_USART_BAUD_RATE);
    delay_ms(100);
    return bt_send_atcmd_with_check("AT+BAUD4\r\n", 1);
}

uint8_t
bt_reset(void) {
    uint32_t i = 500; //初始化i，最长等待5秒
    bt_send_atcmd("RST", NULL);
    do {
        delay_ms(10);
        if (bt_received_data.receive_data_flag == 1 && strstr((char *)bt_received_data.uart_buff, "Ready")) {
            return 0;
        }
    } while (--i);
    return 1;
}

/**
  * @brief  向HC05写入命令，不检查模块的响应
  * @param  command ，要发送的命令
  * @param  arg，命令参数，为0时不带参数，若command也为0时，发送"AT"命令
  * @retval 无
  */
void
bt_send_atcmd(const char* command, const char* arg) {
    if (is_bt_connected()) {
        printf_("Can not send AT as bt is connected\r\n");
        return;
    }
    char str_buf[50];

    bt_delay_ms(10);

    if (arg && arg[0] != 0) {
        sprintf(str_buf, "AT+%s%s\r\n", command, arg);
    } else if (command && command[0] != 0) {
        sprintf(str_buf, "AT+%s\r\n", command);
    } else {
        sprintf(str_buf, "AT\r\n");
    }

    printf_("CMD send:%s", str_buf);

    bt_send_str((char*)str_buf);
}

/**
  * @brief  向HC05模块发送命令并检查OK。只适用于具有OK应答的命令，最长等待5s直到收到OK
  * @param  cmd:命令的完整字符串，需要加\r\n。
  * @param	clean 命令结束后是否清除接收缓冲区，1 清除，0 不清除
  * @template  复位命令：	bt_send_atcmd_with_check("AT+RESET\r\n",1);	
  * @retval 0,设置成功;其他,设置失败.	
  */
uint8_t
bt_send_atcmd_with_check(char* cmd, uint8_t clean) {
    if (is_bt_connected()) {
        printf_("Can not send AT as bt is connected\r\n");
        return 1;
    }
    uint8_t retry = 3;
    uint32_t i;
    uint16_t len;
    char* redata;

    while (retry--) {
        bt_send_str((char*)cmd);
        i = 500;         //初始化i，最长等待5秒
        bt_delay_ms(10); //

        do {
            redata = get_rebuff(&len);
            if (len > 0) {
                if (strstr(redata, "OK")) {
                    printf_("send CMD: %s\r\n", cmd); //打印发送的蓝牙指令和返回信息

                    printf_("recv back: %s\r\n", redata);

                    if (clean == 1) {
                        bt_clean_rebuff();
                    }

                    return 0; //AT指令成功
                }
            }

            bt_delay_ms(10);

        } while (--i); //继续等待

        printf_("send CMD: %s\r\n", cmd); //打印发送的蓝牙指令和返回信息
        printf_("recv back: %s\r\n", redata);
        printf_("BT send CMD fail %d times\r\n", retry); //提示失败重试
    }

    //	BLT_KEY_LOW;
    printf_("BT send CMD fail \r\n");

    if (clean == 1) {
        bt_clean_rebuff();
    }

    return 1; //AT指令失败
}

/**
  * @brief  向HC05模块发送命令并检查OK。只适用于具有OK应答的命令，等待固定时间
  * @param  cmd:命令的完整字符串，需要加\r\n。
  * @param	clean 命令结束后是否清除接收缓冲区，1 清除，0 不清除
  * @param  delayms 等待时间，单位ms
  * @template  查询命令：	bt_send_atcmd_with_wait("AT+INQ\r\n",0, 5000);	
  * @retval 0,设置成功;其他,设置失败.	
  */
uint8_t
bt_send_atcmd_with_wait(char* cmd, uint8_t clean, uint32_t delayms) //Debug
{
    if (is_bt_connected()) {
        printf_("Can not send AT as bt is connected\r\n");
        return 1;
    }
    uint16_t len;
    char* redata;

    bt_send_str((char*)cmd);

    bt_delay_ms(delayms); //固定延时

    redata = get_rebuff(&len);
    if (len > 0) {
        redata[len] = '\0';
        if (redata[0] != 0) {
            printf_("send CMD: %s", cmd);

            printf_("receive %s", redata);
        }
        if (strstr(redata, "OK")) {

            if (clean == 1) {
                bt_clean_rebuff();
            }
            return 0; //AT指令成功
        }
    }

    printf_("BT send CMD fail \r\n");

    if (clean == 1) {
        bt_clean_rebuff();
    }

    return 1; //AT指令失败
}

void
bt_clean_rebuff(void) {
    uint16_t i = BUFF_SIZE + 1;
    bt_received_data.datanum = 0;
    bt_received_data.receive_data_flag = 0;
    bt_received_data.message_type = BT_MESSAGE_NONE;
    while (i) {
        bt_received_data.uart_buff[--i] = 0;
    }
}

/**
  * @brief  使用HC05透传字符串数据
  * @param  str,要传输的字符串
  * @retval 无
  */
void
bt_send_str(char* str) {
    usart1_send_str((uint8_t*)str);
}

/**
 * \brief 发送固定20字节大小的包
 * \param[in] str: 要发送的数据
 */
void
bt_send20_packet(char* str) {
    uint8_t packet[20];
    size_t idx = 0;
    uint8_t* p_packet = packet;
    while (*p_packet) {
        packet[idx++] = *p_packet;
        p_packet++;
    }
    while (idx < 20) {
        packet[idx++] = '\0';
    }
    usart1_send_nbyte(packet, 20);
}

/**
  * @brief  从stream中获取一行字符串到line中
  * @param  line,存储获得行的字符串数组
  * @param  stream，原字符串数据流
  * @param 	max_size，stream的大小 
  * @retval line的长度，若stream中没有‘\0’，'\r'，'\n'，则返回0
  */
int
get_line(char* line, char* stream, size_t max_size) {
    char* p;
    size_t len = 0;
    p = stream;
    while (*p != '\0' && len < max_size) {
        line[len++] = *p;
        p++;
        if ('\n' == *p || '\r' == *p) {
            break;
        }
    }

    if (*p != '\0' && *p != '\n' && *p != '\r') {
        return 0;
    }

    line[len] = '\0';
    return len;
}

/**
  * @brief  替换 rawstr 字符串中的所有 ':'字符
  * @param  rawstr,源字符串
  * @param  delimiter，要替换成的字符
  */
void
raw_replace_char(char* rawstr, char delimiter) {
    do {
        rawstr = strchr(rawstr, ':');
        if (rawstr) {
            *rawstr = delimiter;
        }
    } while (rawstr != NULL);
}

static char*
get_rebuff(uint16_t* len) {
    *len = bt_received_data.datanum;
    return (char*)&bt_received_data.uart_buff;
}
