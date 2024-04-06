/**
 * \file            ir.h
 * \date            3/19/2024
 * \brief           ir head file, used to config ir models
 * \discription     ����ͬһϵͳ�Ĳ�ͬ����ģ�飬���֧��4�����޸Ĳ��ִ���֧�ָ��ࣩ
 * \usuage
 *                  1.���ݻ������úø�ͷ�ļ���
 *                  2.���ݻ�����дir.c��get_one_time_feedback(char* feedback)����
 *                  3.��ʼ��
 *                  4.���� ir_change_operated_ir()�����л�����������ģ��
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
 * \brief ����ģ�鴮��ͨ�Ų�����
 */
#define IR_BAUD_RATE     9600

/**
 * \brief ����ģ���ʼ������������ж����ʼ��ģ�飬�ڴ˺�����ʼ���������
 */
#define ir_usarts_init() usart2_init(IR_BAUD_RATE)

/**
 * \brief ������ʹ�õ�delay���뼶����
 */
#define ir_delay_ms(ms)  delay_ms(ms)

/**
 * \brief �ȴ�������ʱʱ��(ms)
 * \note  ���Բ���
 */
#define IR_TIME_OUT      100

/**
 * \brief �ȴ������ڼ��ط����(ms)
 * \note  ���Բ���
 * ���磺���ó�ʱʱ��Ϊ100�� �ȴ������ڼ��ط����Ϊ10
 *      ����������ڵȴ���Ӧ100ms�������ط�100 / 10 = 10 ��
 */
#define IR_TIME_RESEND   10

/**
 * \brief �豸�Ϲ��ص����к���ģ�鶨��
 */
typedef enum {
    IR1,
    //    IR2,
    //    IR3,
    //    more...
} ir_t;

/**
 * \brief ����ģ�鴮�ڶ���
 * \note �ڴ˶������к���ģ�鴮�ڣ�������ir_tö���е�����һһ��Ӧ
 */
#define IR1_USARTx                        USART2
//#define IR2_USARTx
//#define IR3_USARTx
//more...
/**
 * \end
 */

/**
 * \brief ����ģ�鴮�ڷ������麯������
 * \note �ڴ˶������к���ģ�鴮�ڷ������麯����������ir_tö���е�����һһ��Ӧ
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
 * \brief ����ģ����������
 */
typedef enum {
    IR_CMD_TYPE_EMISSION = 0xF1,
    IR_CMD_TYPE_CHANGE_ADDR,
    IR_CMD_TYPE_BAUD_RATE,
} ir_cmd_type_t;

/**
 * \brief ����ģ������õĲ����ʴ�С
 */
typedef enum {
    IR_BAUD_RATE_4800 = 0x01,
    IR_BAUD_RATE_9600,
    IR_BAUD_RATE_19200,
    IR_BAUD_RATE_57600,
} ir_baud_rate_t;

/**
 * \brief ��ʼ������
 */
void ir_init(void);

/**
 * \brief ���ı���������ģ��
 * \param ir Ҫ�����ĺ���ģ��
 * \note  ����һ����Ҫ���ı�����ģ��ǰֻ��Ҫ����һ��
 *        ����δ���øú�����Ĭ�����в�������IR1��
 */
void ir_change_operated_ir(ir_t ir);

/**
 * \brief ����ģ�鷢���������
 * \param[in] ir_addr: ����ģ���ַ
 * \param[in] high: data1
 * \param[in] low: data2
 * \param[in] command: data3
 */
void ir_emission(uint8_t ir_addr, uint8_t high, uint8_t low, uint8_t command);

/**
 * \brief �ı����ģ��ĵ�ַ
 * \param[in] ir_addr   Ҫ�ı�ĺ���ģ��ĵ�ַ
 * \param[in] new_addr  �µ�ַ
 * \return �����ɹ���1����ʧ�ܣ�0��
 */
char* ir_change_addr(uint8_t ir_addr, uint8_t new_addr);

/**
 * \brief �ı����ģ�鴮��ͨ�Ų�����
 * \param[in] ir_addr       ����ģ���ַ
 * \param[in] ir_baud_rate  Ҫ���õĲ�����
 * \return �����ɹ���1����ʧ�ܣ�0��
 */
char* ir_change_baud_rate(uint8_t ir_addr, ir_baud_rate_t ir_baud_rate);

/**
 * \brief ���Ե�ַ�Ƿ��ģ���ַƥ��
 * \param[in] ir_addr:  Ҫȷ�ϵĵ�ַ
 * \return              ��ַƥ��(1)���߲�ƥ��(0)
 */
uint8_t ir_addr_confirm(uint8_t ir_addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NB_CHARIOT_F103C8T6_IR_H */
