#include "blt.h"
#include <stdio.h>
#include <string.h>
#include "usart_blt.h"
#include "usart.h"

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
    USART1_Init(BLT_USART_BAUD_RATE);
    delay_ms(100);
    return bt_send_atcmd_with_check("AT+VER\r\n", 1);
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

    Usart_SendString(BT_USART, (uint8_t*)str_buf);
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
        Usart_SendString(BT_USART, (uint8_t*)cmd);
        i = 500;                                     //��ʼ��i����ȴ�5��
        bt_delay_ms(10);                           //

        do {
            redata = get_rebuff(&len);
            if (len > 0) {
                if (strstr(redata, "VER")) {
                    printf_("send CMD: %s\r\n", cmd); //��ӡ���͵�����ָ��ͷ�����Ϣ

                    printf_("recv back: %s\r\n", redata);

                    if (clean == 1) {
                        clean_rebuff();
                    }

                    return 0; //ATָ��ɹ�
                }
            }

            bt_delay_ms(10);

        } while (--i); //�����ȴ�

        printf_("send CMD: %s\r\n", cmd); //��ӡ���͵�����ָ��ͷ�����Ϣ
        printf_("recv back: %s\r\n", redata);
        printf_("HC05 send CMD fail %d times\r\n", retry); //��ʾʧ������
    }

    //	BLT_KEY_LOW;
    printf_("HC05 send CMD fail \r\n");

    if (clean == 1) {
        clean_rebuff();
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

    Usart_SendString(BT_USART, (uint8_t*)cmd);

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
                clean_rebuff();
            }
            return 0; //ATָ��ɹ�
        }
    }

    printf_("HC05 send CMD fail ");

    if (clean == 1) {
        clean_rebuff();
    }

    return 1; //ATָ��ʧ��
}

/**
  * @brief  ʹ��HC05͸���ַ�������
  * @param  str,Ҫ������ַ���
  * @retval ��
  */
void
bt_send_str(char* str) {
    Usart_SendString(BT_USART, (uint8_t*)str);
}

/**
  * @brief  ��str�У�������ǰ���prefix�ַ���,
            ��strΪ"abcdefg",prefixΪ"abc"������ñ������󷵻�ָ��"defg"��ָ��
  * @param  str,ԭ�ַ���
  * @param  str_length���ַ�������
  * @param 	prefix��Ҫ�������ַ���
  * @retval ����prefix����ַ���ָ��
  */
static char*
skip_prefix(char* str, size_t str_length, const char* prefix) {

    uint16_t prefix_length = strlen(prefix);

    if (!str || str_length == 0 || !prefix) {
        return 0;
    }

    if (str_length >= prefix_length && strncmp(str, prefix, prefix_length) == 0) {
        return str + prefix_length;
    }

    return 0;
}

/**
  * @brief  ��stream�л�ȡһ���ַ�����line��
  * @param  line,�洢����е��ַ�������
  * @param  stream��ԭ�ַ���������
  * @param 	max_size��stream�Ĵ�С 
  * @retval line�ĳ��ȣ���stream��û�С�\0����'\r'��'\n'���򷵻�0
  */
int
get_line(char* line, char* stream, int max_size) {
    char* p;
    int len = 0;
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

/**
  * @brief  ��ȡԶ�������豸������
  * @param  bltDev �������豸�б�ָ��
  * @retval 0��ȡ�ɹ�����0���ɹ�
  */
uint8_t
get_remote_device_name(void) {
    // uint8_t i;
    // char* redata;
    // uint16_t len;

    // char linebuff[50];
    // uint16_t linelen;
    // char* p;

    // char cmdbuff[100];

    // for (i = 0; i < bltDevList.num; i++) {
    //     raw_replace_char(bltDevList.unpraseAddr[i], ','); //��ַ��ʽ�滻':'Ϊ','
    //     sprintf(cmdbuff, "AT+RNAME?%s\r\n", bltDevList.unpraseAddr[i]);
    //     bt_send_atcmd_with_check(cmdbuff, 0); //���ͻ�ȡ������������

    //     redata = get_rebuff(&len);
    //     if (redata[0] != 0 && strstr(redata, "OK") != 0) {

    //         linelen = get_line(linebuff, redata, len);
    //         if (linelen > 50 && linelen != 0) {
    //             linebuff[linelen] = '\0'; //�����ض�
    //         }

    //         p = skip_prefix(linebuff, linelen, "+RNAME:");
    //         if (p != 0) {
    //             strcpy(bltDevList.name[i], p);
    //         }

    //     } else {
    //         strcpy(bltDevList.name[i], "<���ֻ�ȡʧ��>");
    //         printf_("�������ֻ�ȡʧ�ܣ�\r\n");
    //     }

    //     clean_rebuff(); //��ջ���
    // }

    return 0;
}