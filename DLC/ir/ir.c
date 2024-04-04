#include <stdint-gcc.h>
#include <string.h>
#include "delay.h"
#include "ir.h"

#define IR_BAUD_RATE         9600
#define IR_UNIVERSAL_ADDRESS 0xFA
#define IR_TIME_OUT          100

static char* send_command(uint8_t ir_addr, ir_state_t ir_stata, uint8_t high, uint8_t low, uint8_t command);
static uint8_t get_one_time_feedback(char* feedback);

void
ir_init(void) {
    usart2_init(IR_BAUD_RATE);
}

void
ir_emission(uint8_t ir_addr, uint8_t high, uint8_t low, uint8_t command) {
    send_command(ir_addr, IR_STATE_EMISSION, high, low, command);
}

char*
ir_change_addr(uint8_t ir_addr, uint8_t new_addr) {
    return send_command(ir_addr, IR_STATE_CHANGE_ADDR, new_addr, 0x00, 0x00);
}

char*
ir_change_baud_rate(uint8_t ir_addr, ir_baud_rate_t ir_baud_rate) {
    return send_command(ir_addr, IR_STATE_BAUD_RATE, ir_baud_rate, 0x00, 0x00);
}

uint8_t
ir_addr_confirm(uint8_t ir_addr) {
    return ir_change_addr(ir_addr, ir_addr) == NULL ? 0 : 1;
}

/**
 * \brief           获取一次红外接收器的反馈信息
 *                  此函数检测并获取一次反馈信息，如果成功获取到反馈信息，则将其存储在给定的字符串中。
 * \param[in,out]   feedback:存储反馈信息的字符串
 * \return          如果成功获取到反馈信息，则返回 1，否则返回 0
 */
static uint8_t
get_one_time_feedback(char* feedback) {
    if (weapon_received_data.receive_data_flag == 1 && weapon_received_data.message_type == WEAPON_MESSAGE_FEEDBACK) {
        strncpy(feedback, (char*)weapon_received_data.uart_buff, weapon_received_data.datanum);
        return 1;
    }
    return 0;
}

static char*
send_command(uint8_t ir_addr, ir_state_t ir_stata, uint8_t high, uint8_t low, uint8_t command) {
    static char feedback[64];
    uint8_t time_out = IR_TIME_OUT;
    uint8_t command_arr[5];
    command_arr[0] = ir_addr;
    command_arr[1] = ir_stata;
    command_arr[2] = high;
    command_arr[3] = low;
    command_arr[4] = command;
    do {
        if (time_out % 10 == 0) {
            /* 每10ms重发一次 */
            usart2_send_nbyte(command_arr, 5);
        }
        delay_ms(1);
        time_out--;
    } while (!(time_out == 0 || get_one_time_feedback(feedback)));
    if (time_out == 0) {
        return NULL;
    }
    return feedback;
}
