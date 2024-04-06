/**
 * \file            ir.h
 * \date            3/19/2024
 * \brief           ir head file, used to config ir models
 * \discription     控制同一系统的不同红外模块，最大支持4个（修改部分代码支持更多）
 * \usuage
 *                  1.根据环境配置好该头文件，
 *                  2.根据环境编写ir.c的get_one_time_feedback(char* feedback)函数
 *                  3.初始化
 *                  4.调用 ir_change_operated_ir()函数切换被操作红外模块
 *
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

#ifndef NB_CHARIOT_F103C8T6_IR_H
#define NB_CHARIOT_F103C8T6_IR_H

#include "stm32f10x.h"
#include "usart.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*-------------------------------- Config here --------------------------------*/
/*-------------------------------- Config here --------------------------------*/
/*--------------- Change something according to your environment --------------*/
/*--------------- Change something according to your environment --------------*/

/**
 * \brief 红外模块串口通信波特率
 */
#define IR_BAUD_RATE     9600

/**
 * \brief 红外模块初始化函数，如果有多个初始化模块，在此函数初始化多个串口
 */
#define ir_usarts_init() usart2_init(IR_BAUD_RATE)

/**
 * \brief 该驱动使用的delay毫秒级函数
 */
#define ir_delay_ms(ms)  delay_ms(ms)

/**
 * \brief 等待反馈超时时间(ms)
 * \note  可以不改
 */
#define IR_TIME_OUT      100

/**
 * \brief 等待回馈期间重发间隔(ms)
 * \note  可以不改
 * 例如：设置超时时间为100， 等待回馈期间重发间隔为10
 *      发送命令后，在等待回应100ms内最多会重发100 / 10 = 10 次
 */
#define IR_TIME_RESEND   10

/**
 * \brief 设备上挂载的所有红外模块定义
 */
typedef enum {
    IR1,
    //    IR2,
    //    IR3,
    //    more...
} ir_t;

/**
 * \brief 红外模块串口定义
 * \note 在此定义所有红外模块串口，必须与ir_t枚举中的名称一一对应
 */
#define IR1_USARTx                        USART2
//#define IR2_USARTx
//#define IR3_USARTx
//more...
/**
 * \end
 */

/**
 * \brief 红外模块串口发送数组函数定义
 * \note 在此定义所有红外模块串口发送数组函数，必须与ir_t枚举中的名称一一对应
 */
#define ir1_usart_send_array(array, size) usart2_send_nbyte(array, size)
//#define ir2_usart_send_array(array, size)
//#define ir3_usart_send_array(array, size)
//#define ir4_usart_send_array(array, size)
//more...
/**
 * \end
 */

/*-------------------------------- NO changes here --------------------------------*/
/*-------------------------------- NO changes here --------------------------------*/
/*-------------------------------- NO changes here --------------------------------*/

/**
 * \brief 红外模块命令类型
 */
typedef enum {
    IR_CMD_TYPE_EMISSION = 0xF1,
    IR_CMD_TYPE_CHANGE_ADDR,
    IR_CMD_TYPE_BAUD_RATE,
} ir_cmd_type_t;

/**
 * \brief 红外模块可设置的波特率大小
 */
typedef enum {
    IR_BAUD_RATE_4800 = 0x01,
    IR_BAUD_RATE_9600,
    IR_BAUD_RATE_19200,
    IR_BAUD_RATE_57600,
} ir_baud_rate_t;

/**
 * \brief 初始化函数
 */
void ir_init(void);

/**
 * \brief 更改被操作红外模块
 * \param ir 要操作的红外模块
 * \note  在下一次需要更改被操作模块前只需要调用一次
 *        若从未调用该函数，默认所有操作都在IR1上
 */
void ir_change_operated_ir(ir_t ir);

/**
 * \brief 红外模块发射红外数据
 * \param[in] ir_addr: 红外模块地址
 * \param[in] high: data1
 * \param[in] low: data2
 * \param[in] command: data3
 */
void ir_emission(uint8_t ir_addr, uint8_t high, uint8_t low, uint8_t command);

/**
 * \brief 改变红外模块的地址
 * \param[in] ir_addr   要改变的红外模块的地址
 * \param[in] new_addr  新地址
 * \return 操作成功（1）或失败（0）
 */
char* ir_change_addr(uint8_t ir_addr, uint8_t new_addr);

/**
 * \brief 改变红外模块串口通信波特率
 * \param[in] ir_addr       红外模块地址
 * \param[in] ir_baud_rate  要设置的波特率
 * \return 操作成功（1）或失败（0）
 */
char* ir_change_baud_rate(uint8_t ir_addr, ir_baud_rate_t ir_baud_rate);

/**
 * \brief 测试地址是否和模块地址匹配
 * \param[in] ir_addr:  要确认的地址
 * \return              地址匹配(1)或者不匹配(0)
 */
uint8_t ir_addr_confirm(uint8_t ir_addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_IR_H */
