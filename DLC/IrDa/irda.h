#ifndef __IRDA_H
#define __IRDA_H

#include "stm32f10x.h"
void EXTI_PB11_Config(void);
void IrDa_Init(void);
uint8_t Get_Pulse_Time(void);
uint8_t IrDa_Process(uint8_t ir_byte[]);

#define IRDA_ID                  0

//红外接收使用的GPIO及时钟
#define IRDA_GPIO_PORT           GPIOA
#define IRDA_GPIO_CLK            RCC_APB2Periph_GPIOA
#define IRDA_GPIO_PIN            GPIO_Pin_5
#define IRDA_GPIO_PORT_SOURCE    GPIO_PortSourceGPIOA
#define IRDA_GPIO_PIN_SOURCE     GPIO_PinSource5

//中断相关
#define IRDA_EXTI_LINE           EXTI_Line5
#define IRDA_EXTI_IRQN           EXTI9_5_IRQn
#define IRDA_EXTI_IRQHANDLER_FUN EXTI9_5_IRQHandler

//读取引脚的电平
#define IrDa_DATA_IN()           GPIO_ReadInputDataBit(IRDA_GPIO_PORT, IRDA_GPIO_PIN)

#endif /* __IRDA_H */
