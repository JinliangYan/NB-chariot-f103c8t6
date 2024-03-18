#ifndef __HCSR_H
#define __HCSR_H
#include "delay.h"
#include "stdlib.h"
#include "sys.h"
#include "usart.h"

//������Ӳ���ӿڶ���
#define HCSR04_PORT GPIOB
#define HCSR04_CLK  RCC_APB2Periph_GPIOB
#define HCSR04_TRIG GPIO_Pin_3
#define HCSR04_ECHO GPIO_Pin_4

#define TRIG_Send   PBout(3)
#define ECHO_Reci   PBin(4)

void Hcsr04_NVIC(void);
void Hcsr04_Init(void);
u32 GetEchoTimer(void);            //��ȡ��ʱ��4������ֵ
float Hcsr04GetLength(void);       //ͨ����ʱ��4������ֵ�������
void Hcsr04_Text(void);

#endif
