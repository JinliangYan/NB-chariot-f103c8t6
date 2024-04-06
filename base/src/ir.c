/**
* \file            ir.c
* \date            3/19/2024
* \brief           ir driver
* \note            该驱动的不包括红外外模块的串口接收逻辑，请在别处实现
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

#include <string.h>
#include "delay.h"
#include "ir.h"

static char* send_command(uint8_t ir_addr, ir_cmd_type_t ir_cmd_type, uint8_t high, uint8_t low, uint8_t command);
static uint8_t get_one_time_feedback(char* feedback);

/**
 * \brief 红外模块通用地址
 */
#define IR_UNIVERSAL_ADDRESS 0xFA

/**
 * \brief 对应至ir_t枚举，用来实现预编译控制
 */
#define IR1 0
#define IR2 1
#define IR3 2
#define IR4 3
/**
 * \end
 */

/**
 * \brief 当前操作的红外模块，默认IR1
 */
static ir_t operated_ir;

/**
 * \brief 初始化函数
 */
void
ir_init(void) {
    ir_usarts_init();
    operated_ir = IR1;
}

/**
 * \brief 更改被操作红外模块
 * \param ir 要操作的红外模块
 */
void
ir_change_operated_ir(ir_t ir) {
    if (ir == IR1) {
#ifdef IR1_USARTx
        operated_ir = IR1;
#endif
    } else if (ir == IR2) {
#ifdef IR2_USARTx
        operated_ir = IR2;
#endif
    } else if (ir == IR3) {
#ifdef IR2_USARTx
        operated_ir = IR2;
#endif
    } else if (ir == IR4) {
#ifdef IR2_USARTx
        operated_ir = IR2;
#endif
    }
}

/**
 * \brief 红外模块发射红外数据
 * \param[in] ir_addr: 红外模块地址
 * \param[in] high: data1
 * \param[in] low: data2
 * \param[in] command: data3
 */
void
ir_emission(uint8_t ir_addr, uint8_t high, uint8_t low, uint8_t command) {
    send_command(ir_addr, IR_CMD_TYPE_EMISSION, high, low, command);
}

/**
 * \brief 改变红外模块的地址
 * \param[in] ir_addr   要改变的红外模块的地址
 * \param[in] new_addr  新地址
 * \return 操作成功（1）或失败（0）
 */
char*
ir_change_addr(uint8_t ir_addr, uint8_t new_addr) {
    return send_command(ir_addr, IR_CMD_TYPE_CHANGE_ADDR, new_addr, 0x00, 0x00);
}

/**
 * \brief 改变红外模块串口通信波特率
 * \param[in] ir_addr       红外模块地址
 * \param[in] ir_baud_rate  要设置的波特率
 * \return 操作成功（1）或失败（0）
 */
char*
ir_change_baud_rate(uint8_t ir_addr, ir_baud_rate_t ir_baud_rate) {
    return send_command(ir_addr, IR_CMD_TYPE_BAUD_RATE, ir_baud_rate, 0x00, 0x00);
}

/**
 * \brief 测试地址是否和模块地址匹配
 * \param[in] ir_addr:  要确认的地址
 * \return              地址匹配(1)或者不匹配(0)
 */
uint8_t
ir_addr_confirm(uint8_t ir_addr) {
    /* 通过更改地址指令是否有反馈判断地址是否匹配 */
    /* 注意，这并不会对模块配置做出任何的改变，只起确认作用 */
    return ir_change_addr(ir_addr, ir_addr) == NULL ? 0 : 1;
}

/**
 * \brief   向红外模块发送命令
 * \param[in] ir_addr:   红外模块地址
 * \param[in] ir_stata:  功能模式
 * \param[in] high:      data1
 * \param[in] low:       data2
 * \param[in] command:   data3
 * \return  feedback(F1, F2 or F3)
 */
static char*
send_command(uint8_t ir_addr, ir_cmd_type_t ir_cmd_type, uint8_t high, uint8_t low, uint8_t command) {
    static char feedback[64];
    uint8_t time_out = IR_TIME_OUT;
    uint8_t command_arr[5];
    command_arr[0] = ir_addr;
    command_arr[1] = ir_cmd_type;
    command_arr[2] = high;
    command_arr[3] = low;
    command_arr[4] = command;
    do {
        if (time_out % IR_TIME_RESEND == 0) {
            /* 重发 */
            if (operated_ir == IR1) {
#ifdef ir1_usart_send_array
                ir1_usart_send_array(command_arr, 5);
#endif /* ir1_usart_send_array */
            } else if (operated_ir == IR2) {
#ifdef ir2_usart_send_array
                ir2_usart_send_array(command_arr, 5);
#endif /* ir2_usart_send_array */
            } else if (operated_ir == IR3) {
#ifdef ir3_usart_send_array
                ir3_usart_send_array(command_arr, 5);
#endif /* ir3_usart_send_array */
            } else if (operated_ir == IR4) {
#ifdef ir4_usart_send_array
                ir4_usart_send_array(command_arr, 5);
#endif /* ir4_usart_send_array */
            }
        }
        ir_delay_ms(1);
        time_out--;
    } while (!(time_out == 0 || get_one_time_feedback(feedback)));
    if (time_out == 0) {
        return NULL;
    }
    return feedback;
}

/*--------------------------- You code here---------------------------------------------------*/
/*--------------------------- Modify according to your environment ---------------------------*/

/**
 * \brief           获取一次红外接收器的反馈信息（feedback）
 *                  此函数检测并获取一次反馈信息，如果成功获取到反馈信息，则将其存储在给定的字符串中。
 * \param[in,out]   feedback:存储反馈信息的字符串
 * \return          如果成功获取到反馈信息，则返回 1，否则返回 0
 */
static uint8_t
get_one_time_feedback(char* feedback) {
    if (operated_ir == IR1) {
#ifdef IR1_USARTx
        if (weapon_received_data.receive_data_flag == 1 && weapon_received_data.message_type == WEAPON_MESSAGE_FEEDBACK) {
            strncpy(feedback, (char*)weapon_received_data.uart_buff, weapon_received_data.datanum);
            return 1;
        }
#endif /* IR1_USARTx */
    } else if (operated_ir == IR2) {
#ifdef IR2_USARTx
        if (/* 接收到数据且为feedback数据 */) {
            strncpy(feedback, /* 串口接收数据缓冲区 */, /* 数据大小 */);
            return 1;
        }
#endif /* IR2_USARTx */
    }  else if (operated_ir == IR3) {
#ifdef IR3_USARTx
        if (/* 接收到数据且为feedback数据 */) {
            strncpy(feedback, /* 串口接收数据缓冲区 */, /* 数据大小 */);
            return 1;
        }
#endif /* IR3_USARTx */
    } else if (operated_ir == IR4) {
#ifdef IR4_USARTx
        if (/* 接收到数据且为feedback数据 */) {
            strncpy(feedback, /* 串口接收数据缓冲区 */, /* 数据大小 */);
            return 1;
        }
#endif /* IR4_USARTx */
    }
    return 0;
}


