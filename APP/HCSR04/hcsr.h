#ifndef __HCSR_H
#define __HCSR_H
#include "delay.h"
#include "stdlib.h"
#include "sys.h"
#include "usart.h"

//超声波硬件接口定义
#define HCSR04_PORT GPIOB
#define HCSR04_CLK  RCC_APB2Periph_GPIOB
#define HCSR04_TRIG GPIO_Pin_3
#define HCSR04_ECHO GPIO_Pin_4

#define TRIG_Send   PBout(3)
#define ECHO_Reci   PBin(4)

void Hcsr04_NVIC(void);
void Hcsr04_Init(void);
u32 GetEchoTimer(void);            //获取定时器4计数器值
float Hcsr04GetLength(void);       //通过定时器4计数器值推算距离
void Hcsr04_Text(void);

#endif
