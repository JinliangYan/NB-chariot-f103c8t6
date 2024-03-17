#include "usart.h"
#include "usart_blt.h"
#include "led.h"
#include "mode.h"
#include "printf.h"
#include "string.h"
#include "sys.h"

//ע��:ʹ������ģ��ʱ������ʹ��9600,���ܳ���9600������

typedef enum { NONE, START, TYPE_COMMON, TYPE_AT, MESSAGE, FINISH } receive_state_t;

receive_state_t next_state = FINISH;

bt_received_data_t bt_received_data;

void
USART_SendByte(USART_TypeDef* USARTx, uint16_t Data) //����һ���ֽ�
{
    USARTx->DR = (Data & (uint16_t)0x01FF);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
}

/**************************************************
�������ƣ�USART1_Init(u32 bound)
�������ܣ�����1��ʼ��
��ڲ�����bound  ������
���ز�������
***************************************************/
void
usart1_init_remap(u32 bound) {
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ʹ��AFIOʱ�ӡ���ӳ�����ʹ��AFIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    /* ѡ����ӳ�� */
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); //ʹ��USART1��GPIOAʱ��
    //USART1_TX 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                           //��ʼ��NVIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;                                     //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure);      //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                     //ʹ�ܴ���1
}

/**************************************************
�������ƣ�USART2_Init(u32 bound)
�������ܣ�����2��ʼ��
��ڲ�����bound  ������
���ز�������
***************************************************/
void
USART2_Init(u32 bound) {
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ��USART2ʱ��

    //USART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;         //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);            //��ʼ��GPIOA.2

    //USART2_RX	  GPIOA.3��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             //PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //��ʼ��GPIOA.3

    //Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                           //��ʼ��NVIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;                                     //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);      //��ʼ������2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
    USART_Cmd(USART2, ENABLE);                     //ʹ�ܴ���2
}

/**************************************************
�������ƣ�USART3_Init(u32 bound)
�������ܣ�����3��ʼ��
��ڲ�����bound  ������
���ز�������
***************************************************/
void
USART3_Init(u32 bound) {
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART3ʱ��

    //USART3_TX   GPIOB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //Ƶ��50ZMHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);            //��ʼ��GPIOB.10

    //USART3_RX	  GPIOB.11��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //��ʼ��GPIOB.11

    //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                           //��ʼ��NVIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;                                     //���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure);      //��ʼ������3
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
    USART_Cmd(USART3, ENABLE);                     //ʹ�ܴ���3
}

/**************************************************
�������ƣ�USART1_IRQHandler(void)
�������ܣ�����1�жϺ���
��ڲ�������
���ز�������
***************************************************/
void
USART1_IRQHandler(void) {
    u8 temp;
    static u8 idx = 0;
    static uint8_t temp_buf1[UART_BUFF_SIZE];

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        temp = USART_ReceiveData(USART1);

        /* ���յ���ʼλ */
        if (temp == PACKAGE_START_FLAG) {
            clean_rebuff(); /* ��ս��ջ�����, ׼������ */
            next_state = TYPE_COMMON;
            return;
        }
        if (temp == PACKAGE_AT_FLAG) {
            clean_rebuff(); /* ��ս��ջ�����, ׼������ */
            bt_received_data.message_type = MESSAGE_AT_COMMAND;
            next_state = TYPE_AT;
            return;
        }

        /* ������Ϣ���� */
        if (next_state == TYPE_COMMON) {
            /* ƥ����Ϣ���� */
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

        /* AT���� */
        if (bt_received_data.message_type == MESSAGE_AT_COMMAND) {
            /* ����0 */
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

        /* ���յ���ֹλ */
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

        /* ������Ϣ���� */
        temp_buf1[idx++] = temp;
    }
}

/*************************���ڷ��ͺ���*************************************/

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