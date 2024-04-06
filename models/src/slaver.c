/**
 * \file            slaver.c
 * \date            4/3/2024
 * \brief           与副板通信
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

static void slaver_video_wifi_config_connect(const char* ssid, const char* pwd);
static void slaver_video_quick_connect(void);
static void get_video_message(void);
static void get_model_message(void);

#define SLAVER_BAUD_RATE 9600
#define SSID             ""
#define PASSWORD         ""
#define TIME_OUT         strstr(message, "Connect_TimeOut")

static char message[1024];

void
slaver_init(void) {
    usart3_init(SLAVER_BAUD_RATE);
    slaver_video_start();

    slaver_video_wifi_config_connect(SSID, PASSWORD);
}

void
slaver_video_start(void) {
    usart3_send_str("$VStart*");
    do {
        get_video_message();
    } while (!strstr(message, "READY"));
}

void
slaver_video_restart(void) {
    usart3_send_str("VRestart*");
    do {
        get_video_message();
    } while (!strstr(message, "READY"));
}

void
slaver_video_disconnect(void) {
    usart3_send_str("$VDisconnect*");
    do {
        get_video_message();
    } while (!strstr(message, "Disconnect_Finish"));
}

/**
 * \brief               向从板发送验证模块地址的命令
 *                      格式：$MConfirm[model]Addr[addr]*
 *                         -model:  Defence || Move
 *                         -addr:   待求证的地址， 大小1字节
 * \param[in] model： Model to be confirm, can be Defence or Move
 * \param[in] addr:  Addr to be confirm, can be any 1byte number
 * \return              If matches return 1, otherwise 0
 * \note                从机接口实现要求:
 *                          返回Yes || No
 *                          在一定时间内等待模块回应
 *                          如果地址符合， 发送$MYes*
 *                          超时则不符合， 返回$MNo*
 */
uint8_t slaver_model_addr_confirm(char* model, uint8_t addr) {
    char command[1024];
    sprintf(command, "$MConfirm%sAddr%02X*", model, addr);
    usart3_send_str(command);
    do {
        get_model_message();
    } while (!strstr("YesNo", message));
    if (strchr(message, 'Y'))
        return 1;
    return 0;
}

static void
slaver_video_wifi_config_connect(const char* ssid, const char* pwd) {
    char command[1024];
    usart3_send_str("$VWificonfig*");
    sprintf(command, "$V%sP%s\n", ssid, pwd);
    usart3_send_str(command);
    do {
        get_video_message();
    } while (!strstr(message, "Connect_OK") || TIME_OUT);
    do {
        get_video_message();
    } while (!strstr(message, "Wificonfig_Finish"));
}

static void
slaver_video_quick_connect(void) {
    usart3_send_str("$VQuickconnect*");
    do {
        get_video_message();
    } while (!strstr(message, "Quickconnect_Finish") || TIME_OUT);
}

static void
get_video_message(void) {
    while (slaver_received_data.receive_data_flag == 0 && slaver_received_data.message_type != SLAVER_MESSAGE_VIDEO) {
        ;
    }
    strcpy(message, (char*)slaver_received_data.uart_buff);
    slaver_received_data.receive_data_flag = 0;
}

static void
get_model_message(void) {
    while (slaver_received_data.receive_data_flag == 0 && slaver_received_data.message_type != SLAVER_MESSAGE_MODEL) {
        ;
    }
    strcpy(message, (char*)slaver_received_data.uart_buff);
    slaver_received_data.receive_data_flag = 0;
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
