#include "usart.h"
#include "led.h"
#include "printf.h"
#include "string.h"
#include "sys.h"

//注意:使用蓝牙模块时波特率使用9600,不能超过9600波特率

u8 flag = 0;
u8 mode_flag = 0;

//char Lx_Buf[10]={0};

char Lx_Buf[10] = {0};   //左摇杆数据接收缓冲区
char Rx_Buf[10] = {0};   //右摇杆数据接收缓冲区
char Weapon_Buf[10] = {0};   //武器方向数据接收缓冲区
char Pitch_Roll_Buf[20]; //APP偏航角数据接收缓冲区

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
USART1_Init(u32 bound) {
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //使能USART1，GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;         //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //频率50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //初始化GPIOA.9

    //USART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化GPIOA.10

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
    static u8 t = 0, n1 = 0, n2 = 0;
    static u8 i = 0, j = 0, k = 0;
    static char temp_buf1[10] = {0}, temp_buf2[10] = {0}, temp_buf3[20] = {0};
    static u8 Lx_flag = 0, Rx_flag = 0, Pr_flag = 0; //左右摇杆，陀螺仪接收数据标志位
    static uint8_t Wx_flag = 0; /*武器接收数据标志位*/

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        temp = USART_ReceiveData(USART1);
//        printf_("UART1 receives: %c\r\n", temp);
        if (temp == 'a') {
            mode_flag = 1;  /*APP遥控模式*/
        } else if (temp == 'b') {
            mode_flag = 2;  /*APP重力模式*/
        } else if (temp == 'c') {
            mode_flag = 3;  /*避障模式*/
        } else if (temp == 'd') {
            mode_flag = 4;  /*跟随模式*/
        } else if (temp == 'e') {
            mode_flag = 5;  /*呼吸灯*/
        } else if (temp == 'f') {
            mode_flag = 6;  /*流水灯*/
        } else if (temp == 'g') {
            mode_flag = 7;  /*闪烁灯*/
        } else {
            mode_flag = 1;
        }

//        printf_("modeflag = %d\r\n", mode_flag);

        if (t == 0 && mode_flag == 4) //过滤第一次mode_flag=4
        {
            mode_flag = 0;
            t = 1;
        }

        if (mode_flag != 2) //当模式不为重力感应模式时(遥控模式)
        {
            if (temp == 'L' && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f') //接收到帧头为L的一帧数据
            {
                Lx_flag = 1; //标志位置1
            }
            if (Lx_flag == 1 && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f') //开始接收这一帧数据
            {
                temp_buf1[i] = temp;
                i++;
                if (temp == '*') //帧尾为*时一帧数据接收完毕
                {
                    if (n1 == 0) //过滤第一次摇杆数据
                    {
                        memset(Lx_Buf, 0, 10);
                        memset(temp_buf1, 0, 10);
                        n1 = 1;
                    }
                    strcpy(Lx_Buf, temp_buf1);
                    Lx_flag = 0;
                    i = 0;
//                    printf_("UART1 receives: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \r\n", Lx_Buf[0], Lx_Buf[1], Lx_Buf[2], Lx_Buf[3], Lx_Buf[4], Lx_Buf[5], Lx_Buf[6], Lx_Buf[7], Lx_Buf[8], Lx_Buf[9]);
                }
            }

            if (temp == 'R' && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f') //接收到帧头为R的一帧数据
            {
                Rx_flag = 1;
            }
            if (Rx_flag == 1 && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f') //开始接收这一帧数据
            {
                temp_buf2[j] = temp;
                j++;
                if (temp == '*') //帧尾为*时一帧数据接收完毕
                {
                    if (n2 == 0) //过滤第一次摇杆数据
                    {
                        memset(Rx_Buf, 0, 10);
                        memset(temp_buf2, 0, 10);
                        n2 = 1;
                    }
                    strcpy(Rx_Buf, temp_buf2);
                    Rx_flag = 0;
                    j = 0;
//                    printf_("UART1 receives: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \r\n\r\n", Rx_Buf[0], Rx_Buf[1], Rx_Buf[2], Rx_Buf[3], Rx_Buf[4], Rx_Buf[5], Rx_Buf[6], Rx_Buf[7], Rx_Buf[8], Rx_Buf[9]);
                }
            }

            /*武器摇杆数据帧接收*/
            if (temp == 'W' && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f')
            {
                Wx_flag = 1;
            }
            if (Wx_flag == 1 && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f') //开始接收这一帧数据
            {
                temp_buf2[j] = temp;
                j++;
                if (temp == '*') //帧尾为*时一帧数据接收完毕
                {
                    if (n2 == 0) //过滤第一次摇杆数据
                    {
                        memset(Weapon_Buf, 0, 10);
                        memset(temp_buf2, 0, 10);
                        n2 = 1;
                    }
                    strcpy(Weapon_Buf, temp_buf2);
                    Wx_flag = 0;
                    j = 0;
//                    printf_("UART1 receives: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X \r\n\r\n", Weapon_Buf[0], Weapon_Buf[1], Weapon_Buf[2], Weapon_Buf[3], Weapon_Buf[4], Weapon_Buf[5], Weapon_Buf[6], Weapon_Buf[7], Weapon_Buf[8], Weapon_Buf[9]);
                }
            }
        }
        /*重力感应模式*/
        else {
            if (temp == 'A') //接收到帧头为A的一帧数据(重力感应模式数据)
            {
                Pr_flag = 1;
                memset(Pitch_Roll_Buf, 0, 20);
                memset(temp_buf3, 0, 20);
            }
            if (Pr_flag == 1 && temp != 'a' && temp != 'b' && temp != 'c' && temp != 'd' && temp != 'e'
                && temp != 'f') {
                temp_buf3[k] = temp;
                k++;
                if (temp == '*') {
                    strcpy(Pitch_Roll_Buf, temp_buf3);
                    Pr_flag = 0;
                    k = 0;
                }
            }
        }
    }
}

/**************************************************
函数名称：USART2_IRQHandler(void) 
函数功能：串口2中断函数
入口参数：无
返回参数：无
***************************************************/
void
USART2_IRQHandler(void) {
    u8 temp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        temp = USART_ReceiveData(USART2);
        switch (temp) {
            case 'a': flag = 1; break;
            case 'b': flag = 2; break;
        }
    }
    USART_SendByte(USART2, temp);
}

/**************************************************
函数名称：USART3_IRQHandler(void) 
函数功能：串口3中断函数
入口参数：无
返回参数：无
***************************************************/
void
USART3_IRQHandler(void) {
    u8 temp;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        temp = USART_ReceiveData(USART3);
        switch (temp) {
            case 'a': flag = 1; break;
            case 'b': flag = 2; break;
        }
    }
    USART_SendByte(USART3, temp);
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

/**************************************************
函数名称：fputc(int ch,FILE *f)
函数功能：串口重定向
入口参数：无
返回参数：无
***************************************************/
#pragma import(__use_no_semihosting)

struct __FILE {
    int handle;
};

FILE __stdout;

_sys_exit(int x) { x = x; }

int
fputc(int ch, FILE* f) {
    USART_SendData(USART1, (uint8_t)ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
    return (ch);
}

int
fgetc(FILE* f) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        ;
    return ((int)USART_ReceiveData(USART1));
}
