#ifndef CONTROL_H
#define CONTROL_H
#include "delay.h"
#include "hcsr.h"
#include "math.h"
#include "motor.h"
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "ws2812b.h"

/**
 * \brief       HSV×ªRGBµÄÑÕÉ«
*/
extern float RGB_R, RGB_G, RGB_B;

void movement_control(void);
void mode_switch(void);
void avoid(void);
void follow_mode(void);
void rgb_show(void);
void control(void);

#endif
