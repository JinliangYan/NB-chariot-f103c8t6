#include "usart.h"
#include "usart_blt.h"
#include "led.h"
#include "mode.h"
#include "printf.h"
#include "string.h"
#include "sys.h"

//注意:使用蓝牙模块时波特率使用9600,不能超过9600波特率

typedef enum { NONE, START, TYPE_COMMON, TYPE_AT, MESSAGE, FINISH } receive_state_t;

receive_state_t next_state = FINISH;

bt_received_data_t bt_received_data;

void
USART_SendByte(USART_TypeDef* USARTx, uint16_t Data) //发送一个字节
{
    USARTx->DR = (Data & (uint16_t)0x01FF);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
}

/**************************************************
函数名称：USART1_Init(u32 bound)
函数功能：串口1初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void
usart1_init_remap(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能AFIO时钟。重映射必须使能AFIO时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* 选择重映射 */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟
    //USART1_TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //初始化NVIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                                     //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式

    USART_Init(USART1, &USART_InitStructure);      //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_Cmd(USART1, ENABLE);                     //使能串口1
}

/**************************************************
函数名称：USART2_Init(u32 bound)
函数功能：串口2初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void
USART2_Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;         //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //初始化GPIOA.2

    //USART2_RX	  GPIOA.3初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             //PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化GPIOA.3

    //Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //初始化NVIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                                     //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式

    USART_Init(USART2, &USART_InitStructure);      //初始化串口2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_Cmd(USART2, ENABLE);                     //使能串口2
}

/**************************************************
函数名称：USART3_Init(u32 bound)
函数功能：串口3初始化
入口参数：bound  波特率
返回参数：无
***************************************************/
void
USART3_Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3时钟

    //USART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //初始化GPIOB.10

    //USART3_RX	  GPIOB.11初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //初始化GPIOB.11

    //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //初始化NVIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;                                     //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式

    USART_Init(USART3, &USART_InitStructure);      //初始化串口3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启串口接受中断
    USART_Cmd(USART3, ENABLE);                     //使能串口3
}

/**************************************************
函数名称：USART1_IRQHandler(void)
函数功能：串口1中断函数
入口参数：无
返回参数：无
***************************************************/
void
USART1_IRQHandler(void) {
    u8 temp;
    static u8 idx = 0;
    static uint8_t temp_buf1[UART_BUFF_SIZE];

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        temp = USART_ReceiveData(USART1);

        /* 接收到起始位 */
        if (temp == PACKAGE_START_FLAG) {
            clean_rebuff(); /* 清空接收缓冲区, 准备接收 */
            next_state = TYPE_COMMON;
            return;
        }
        if (temp == PACKAGE_AT_FLAG) {
            clean_rebuff(); /* 清空接收缓冲区, 准备接收 */
            bt_received_data.message_type = MESSAGE_AT_COMMAND;
            next_state = TYPE_AT;
            return;
        }

        /* 接收消息类型 */
        if (next_state == TYPE_COMMON) {
            /* 匹配消息类型 */
            switch (temp) {
                case 'L': bt_received_data.message_type = MESSAGE_LEFT_JOYSTICK; break;
                case 'R': bt_received_data.message_type = MESSAGE_RIGHT_JOYSTICK; break;
                case 'W': bt_received_data.message_type = MESSAGE_WEAPON_JOYSTICK; break;
                case 'T': bt_received_data.message_type = MESSAGE_TEXT; break;
                case 'M': bt_received_data.message_type = MESSAGE_MODE_SWITCH; break;
                case 'A': bt_received_data.message_type = MESSAGE_WEAPON_ATTACK; break;
                default: break;
            }
            next_state = MESSAGE;
            return;
        }

        /* AT接收 */
        if (bt_received_data.message_type == MESSAGE_AT_COMMAND) {
            /* 过滤0 */
            if (temp == 0) {
                return;
            }
            bt_received_data.uart_buff[idx++] = temp;
            bt_received_data.datanum = idx;
            if (temp == '\n') {
                bt_received_data.receive_data_flag = 1;
                bt_received_data.datanum -= 2;
                idx = 0;
                next_state = START;
            }
            return;
        }

        /* 接收到终止位 */
        if (temp == PACKAGE_END_FLAG) {
            bt_received_data.receive_data_flag = 1;
            next_state = START;
            strcpy(bt_received_data.uart_buff, temp_buf1);
            bt_received_data.datanum = idx;
            bt_received_data.receive_data_flag = 1;
            idx = 0;
            return;
        }


        if (next_state != MESSAGE) {
            next_state = NONE;
            return;
        }

        /* 接收消息内容 */
        temp_buf1[idx++] = temp;
    }
}

/*************************串口发送函数*************************************/

void
USART1_Send_Byte(u8 Data) {
    USART_SendData(USART1, Data);
    return;
}

void
USART1_Send_nByte(u8* Data, u16 size) {
    u16 i = 0;
    for (i = 0; i < size; i++) {
        USART_SendData(USART1, Data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
    }
    return;
}

void
USART1_Send_Str(u8* Data) {
    while (*Data) {
        USART_SendData(USART1, *Data++);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
    }
    return;
}

void
USART2_Send_Byte(u8 Data) {
    USART_SendData(USART2, Data);
    return;
}

void
USART2_Send_nByte(u8* Data, u16 size) {
    u16 i = 0;
    for (i = 0; i < size; i++) {
        USART_SendData(USART2, Data[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
            ;
    }
    return;
}

void
USART2_Send_Str(u8* Data) {
    while (*Data) {
        USART_SendData(USART2, *Data++);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
            ;
    }
    return;
}

void
USART3_Send_Byte(u8 Data) {
    USART_SendData(USART3, Data);
    return;
}

void
USART3_Send_nByte(u8* Data, u16 size) {
    u16 i = 0;
    for (i = 0; i < size; i++) {
        USART_SendData(USART3, Data[i]);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
            ;
    }
    return;
}

void
USART3_Send_Str(u8* Data) {
    while (*Data) {
        USART_SendData(USART3, *Data++);
        while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
            ;
    }
    return;
}