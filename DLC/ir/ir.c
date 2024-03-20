#include "ir.h"

void
ir_init(void) {
    usart2_init(IR_BAUD_RATE);
}

void
ir_emission(uint8_t high, uint8_t low, uint8_t command) {
    uint8_t command_arr[5] = {IR_ADDRESS, IR_STAT_EMISSION, high, low, command};
    usart2_send_nbyte(command_arr, 5);
}
