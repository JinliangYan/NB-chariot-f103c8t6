#include "blt.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

static char* get_rebuff(uint16_t* len);


/**
  * @brief  ��ʼ��GPIO�����HC05ģ��
  * @param  ��
  * @retval HC05״̬��0 ��������0�쳣
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
    uint32_t i = 500; //��ʼ��i����ȴ�5��
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
  * @brief  ��HC05д����������ģ�����Ӧ
  * @param  command ��Ҫ���͵�����
  * @param  arg�����������Ϊ0ʱ������������commandҲΪ0ʱ������"AT"����
  * @retval ��
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
  * @brief  ��HC05ģ�鷢��������OK��ֻ�����ھ���OKӦ��������ȴ�5sֱ���յ�OK
  * @param  cmd:����������ַ�������Ҫ��\r\n��
  * @param	clean ����������Ƿ�������ջ�������1 �����0 �����
  * @template  ��λ���	bt_send_atcmd_with_check("AT+RESET\r\n",1);	
  * @retval 0,���óɹ�;����,����ʧ��.	
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
        i = 500;         //��ʼ��i����ȴ�5��
        bt_delay_ms(10); //

        do {
            redata = get_rebuff(&len);
            if (len > 0) {
                if (strstr(redata, "OK")) {
                    printf_("send CMD: %s\r\n", cmd); //��ӡ���͵�����ָ��ͷ�����Ϣ

                    printf_("recv back: %s\r\n", redata);

                    if (clean == 1) {
                        bt_clean_rebuff();
                    }

                    return 0; //ATָ��ɹ�
                }
            }

            bt_delay_ms(10);

        } while (--i); //�����ȴ�

        printf_("send CMD: %s\r\n", cmd); //��ӡ���͵�����ָ��ͷ�����Ϣ
        printf_("recv back: %s\r\n", redata);
        printf_("BT send CMD fail %d times\r\n", retry); //��ʾʧ������
    }

    //	BLT_KEY_LOW;
    printf_("BT send CMD fail \r\n");

    if (clean == 1) {
        bt_clean_rebuff();
    }

    return 1; //ATָ��ʧ��
}

/**
  * @brief  ��HC05ģ�鷢��������OK��ֻ�����ھ���OKӦ�������ȴ��̶�ʱ��
  * @param  cmd:����������ַ�������Ҫ��\r\n��
  * @param	clean ����������Ƿ�������ջ�������1 �����0 �����
  * @param  delayms �ȴ�ʱ�䣬��λms
  * @template  ��ѯ���	bt_send_atcmd_with_wait("AT+INQ\r\n",0, 5000);	
  * @retval 0,���óɹ�;����,����ʧ��.	
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

    bt_delay_ms(delayms); //�̶���ʱ

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
            return 0; //ATָ��ɹ�
        }
    }

    printf_("BT send CMD fail \r\n");

    if (clean == 1) {
        bt_clean_rebuff();
    }

    return 1; //ATָ��ʧ��
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
  * @brief  ʹ��HC05͸���ַ�������
  * @param  str,Ҫ������ַ���
  * @retval ��
  */
void
bt_send_str(char* str) {
    usart1_send_str((uint8_t*)str);
}

/**
 * \brief ���͹̶�20�ֽڴ�С�İ�
 * \param[in] str: Ҫ���͵�����
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
  * @brief  ��stream�л�ȡһ���ַ�����line��
  * @param  line,�洢����е��ַ�������
  * @param  stream��ԭ�ַ���������
  * @param 	max_size��stream�Ĵ�С 
  * @retval line�ĳ��ȣ���stream��û�С�\0����'\r'��'\n'���򷵻�0
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
  * @brief  �滻 rawstr �ַ����е����� ':'�ַ�
  * @param  rawstr,Դ�ַ���
  * @param  delimiter��Ҫ�滻�ɵ��ַ�
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
