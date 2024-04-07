/**
 * \file            slaver.c
 * \date            4/3/2024
 * \brief           �븱��ͨ��
 */

/*
 * Copyright (c) 2024 JinLiang YAN
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of NB-chariot-f103c8t6.
 *
 * Author:          JinLiang YAN <yanmiku0206@outlook.com>
 */

#include "slaver.h"
#include "string.h"
#include "usart.h"
#include "delay.h"

static uint8_t slaver_video_wifi_config_connect(const char* ssid, const char* pwd);
static uint8_t slaver_video_quick_connect(void);
static uint8_t get_once_video_message(void);
static uint8_t get_once_model_message(void);

#define SLAVER_BAUD_RATE 9600
#define SSID             ""
#define PASSWORD         ""
#define TIME_OUT_SLAVER  strstr(message, "Connect_TimeOut")

#define TIME_OUT      1000
#define TIME_RESEND   300

static char message[1024];

void
slaver_init(void) {
    usart3_init(SLAVER_BAUD_RATE);
    slaver_video_start();

    slaver_video_wifi_config_connect(SSID, PASSWORD);
}

uint8_t
slaver_video_start(void) {
    uint16_t time_out = TIME_OUT;
    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str("$VStart*");
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_model_message() || time_out == 0));

    if (strstr(message, "READY")) {
        return 1;
    }
    return 0;
}

uint8_t
slaver_video_restart(void) {
    uint16_t time_out = TIME_OUT;
    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str("VRestart*");
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_model_message() || time_out == 0));

    if (strstr(message, "READY")) {
        return 1;
    }
    return 0;
}

uint8_t
slaver_video_disconnect(void) {
    uint16_t time_out = TIME_OUT;
    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str("$VDisconnect*");
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_model_message() || time_out == 0));

    if (strstr(message, "Disconnect_Finish")) {
        return 1;
    }
    return 0;
}

/**
 * \brief               ��Ӱ巢����֤ģ���ַ������
 *                      ��ʽ��$MConfirm[model]Addr[addr]*
 *                         -model:  Defence || Move
 *                         -addr:   ����֤�ĵ�ַ�� ��С1�ֽ�
 * \param[in] model�� Model to be confirm, can be Defence or Move
 * \param[in] addr:  Addr to be confirm, can be any 1byte number
 * \return              If matches return 1, otherwise 0
 * \note                �ӻ��ӿ�ʵ��Ҫ��:
 *                          ����Yes || No
 *                          ��һ��ʱ���ڵȴ�ģ���Ӧ
 *                          �����ַ���ϣ� ����$MYes*
 *                          ��ʱ�򲻷��ϣ� ����$MNo*
 */
uint8_t slaver_model_addr_confirm(char* model, uint8_t addr) {
    uint16_t time_out = TIME_OUT;
    char command[1024];

    sprintf(command, "$MConfirm%sAddr%02X*", model, addr);

    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str(command);
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_model_message() || time_out == 0));

    if (strchr(message, 'Y'))
        return 1;
    return 0;
}

/**
 * \brief ��������ͷwifi
 * \param ssid wifi ssid
 * \param pwd wifi pwd
 * \return ���óɹ�����1, ʧ�ܷ���0
 */
static uint8_t
slaver_video_wifi_config_connect(const char* ssid, const char* pwd) {
    uint16_t time_out = TIME_OUT;

    char command[1024];
    sprintf(command, "$V%sP%s\n", ssid, pwd);

    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str("$VWificonfig*");
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_video_message() || time_out == 0));

    if (!strstr(message, "Connect_OK")) {
        return 0;
    }

    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str(command);
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_video_message() || time_out == 0));

    if (!strstr(message, "Wificonfig_Finish")) {
        return 0;
    }

    return 1;
}

static uint8_t
slaver_video_quick_connect(void) {
    uint16_t time_out = TIME_OUT;

    do {
        if (time_out % TIME_RESEND == 0) {
            usart3_send_str("$VQuickconnect*");
        }
        delay_ms(1);
        time_out--;
    } while (!(get_once_video_message() || time_out == 0));

    if (strstr(message, "Quickconnect_Finish")) {
        return 1;
    }
    return 0;
}

/**
 * \brief ���һ���Ƿ���յ���Ƶ��Ϣ, ���������Ϣ���Ƶ� message ��.
 * \return ����Ƶ��Ϣ����1, û�з���0
 */
static uint8_t
get_once_video_message(void) {
    if (slaver_received_data.receive_data_flag == 1 && slaver_received_data.message_type == SLAVER_MESSAGE_VIDEO) {
        strcpy(message, (char*)slaver_received_data.uart_buff);
        slaver_received_data.receive_data_flag = 0;
        return 1;
    }
    return 0;
}

/**
 * \brief ���һ���Ƿ���յ�ģ����Ϣ, ���������Ϣ���Ƶ� message ��.
 * \return ��ģ����Ϣ����1, û�з���0
 */
static uint8_t
get_once_model_message(void) {
    if (slaver_received_data.receive_data_flag == 1 && slaver_received_data.message_type == SLAVER_MESSAGE_MODEL) {
        strcpy(message, (char*)slaver_received_data.uart_buff);
        slaver_received_data.receive_data_flag = 0;
        return 1;
    }
    return 0;
}

void
slaver_clean_rebuff(void) {
    uint16_t i = BUFF_SIZE + 1;
    slaver_received_data.datanum = 0;
    slaver_received_data.receive_data_flag = 0;
    slaver_received_data.message_type = SLAVER_MESSAGE_NONE;
    while (i) {
        slaver_received_data.uart_buff[--i] = 0;
    }
}
