#include "ir.h"

void
ir_init(void) {
    usart2_init(IR_BAUD_RATE);
}

void
ir_emission(uint8_t high, uint8_t low, uint8_t command) {
    usart2_send_byte(IR_ADDRESS);
    usart2_send_byte(IR_STAT_EMISSION);

    usart2_send_byte(high);
    usart2_send_byte(low);
    usart2_send_byte(command);
}