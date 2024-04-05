#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

void pwm1_init(u32 arr, u32 psc);
void pwm2_init(u32 arr, u32 psc);
void pwm3_init(u32 arr, u32 psc);
void pwm3_init_remap(u32 arr, u32 psc);
void pwm4_init(u32 arr, u32 psc);
// void PWM_Init(u32 arr, u32 psc);
// void PWM_Text(void);
// int PWM(int angle);

#endif
