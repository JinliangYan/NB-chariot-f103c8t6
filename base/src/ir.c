/**
* \file            ir.c
* \date            3/19/2024
* \brief           ir driver
* \note            �������Ĳ�����������ģ��Ĵ��ڽ����߼������ڱ�ʵ��
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
 * \brief ����ģ��ͨ�õ�ַ
 */
#define IR_UNIVERSAL_ADDRESS 0xFA

/**
 * \brief ��Ӧ��ir_tö�٣�����ʵ��Ԥ�������
 */
#define IR1 0
#define IR2 1
#define IR3 2
#define IR4 3
/**
 * \end
 */

/**
 * \brief ��ǰ�����ĺ���ģ�飬Ĭ��IR1
 */
static ir_t operated_ir;

/**
 * \brief ��ʼ������
 */
void
ir_init(void) {
    ir_usarts_init();
    operated_ir = IR1;
}

/**
 * \brief ���ı���������ģ��
 * \param ir Ҫ�����ĺ���ģ��
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
 * \brief ����ģ�鷢���������
 * \param[in] ir_addr: ����ģ���ַ
 * \param[in] high: data1
 * \param[in] low: data2
 * \param[in] command: data3
 */
void
ir_emission(uint8_t ir_addr, uint8_t high, uint8_t low, uint8_t command) {
    send_command(ir_addr, IR_CMD_TYPE_EMISSION, high, low, command);
}

/**
 * \brief �ı����ģ��ĵ�ַ
 * \param[in] ir_addr   Ҫ�ı�ĺ���ģ��ĵ�ַ
 * \param[in] new_addr  �µ�ַ
 * \return �����ɹ���1����ʧ�ܣ�0��
 */
char*
ir_change_addr(uint8_t ir_addr, uint8_t new_addr) {
    return send_command(ir_addr, IR_CMD_TYPE_CHANGE_ADDR, new_addr, 0x00, 0x00);
}

/**
 * \brief �ı����ģ�鴮��ͨ�Ų�����
 * \param[in] ir_addr       ����ģ���ַ
 * \param[in] ir_baud_rate  Ҫ���õĲ�����
 * \return �����ɹ���1����ʧ�ܣ�0��
 */
char*
ir_change_baud_rate(uint8_t ir_addr, ir_baud_rate_t ir_baud_rate) {
    return send_command(ir_addr, IR_CMD_TYPE_BAUD_RATE, ir_baud_rate, 0x00, 0x00);
}

/**
 * \brief ���Ե�ַ�Ƿ��ģ���ַƥ��
 * \param[in] ir_addr:  Ҫȷ�ϵĵ�ַ
 * \return              ��ַƥ��(1)���߲�ƥ��(0)
 */
uint8_t
ir_addr_confirm(uint8_t ir_addr) {
    /* ͨ�����ĵ�ַָ���Ƿ��з����жϵ�ַ�Ƿ�ƥ�� */
    /* ע�⣬�Ⲣ�����ģ�����������κεĸı䣬ֻ��ȷ������ */
    return ir_change_addr(ir_addr, ir_addr) == NULL ? 0 : 1;
}

/**
 * \brief   �����ģ�鷢������
 * \param[in] ir_addr:   ����ģ���ַ
 * \param[in] ir_stata:  ����ģʽ
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
            /* �ط� */
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
 * \brief           ��ȡһ�κ���������ķ�����Ϣ��feedback��
 *                  �˺�����Ⲣ��ȡһ�η�����Ϣ������ɹ���ȡ��������Ϣ������洢�ڸ������ַ����С�
 * \param[in,out]   feedback:�洢������Ϣ���ַ���
 * \return          ����ɹ���ȡ��������Ϣ���򷵻� 1�����򷵻� 0
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
        if (/* ���յ�������Ϊfeedback���� */) {
            strncpy(feedback, /* ���ڽ������ݻ����� */, /* ���ݴ�С */);
            return 1;
        }
#endif /* IR2_USARTx */
    }  else if (operated_ir == IR3) {
#ifdef IR3_USARTx
        if (/* ���յ�������Ϊfeedback���� */) {
            strncpy(feedback, /* ���ڽ������ݻ����� */, /* ���ݴ�С */);
            return 1;
        }
#endif /* IR3_USARTx */
    } else if (operated_ir == IR4) {
#ifdef IR4_USARTx
        if (/* ���յ�������Ϊfeedback���� */) {
            strncpy(feedback, /* ���ڽ������ݻ����� */, /* ���ݴ�С */);
            return 1;
        }
#endif /* IR4_USARTx */
    }
    return 0;
}


