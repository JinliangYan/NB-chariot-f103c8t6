/*** 
 * @Author: Jinliang miku.cy@foxmail.com
 * @Date: 2024-03-14 20:40:51
 * @LastEditors: Jinliang miku.cy@foxmail.com
 * @LastEditTime: 2024-03-17 11:09:00
 * @FilePath: \NB-chariot-f103c8t6\USER\mode.h
 * @Description: С��ģʽ����ͷ�ļ�
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
/**
 * \brief   С��ģʽ����
 * \note    ����С��ģʽ�жϺ��л�
 */
typedef enum {
    CONTROL_MODE_JOYSTICK,           /*!< ҡ�˿���ģʽ */
    CONTROL_MODE_GRAVITY,            /*!< �����ǿ���ģʽ */
    CONTROL_MODE_EVADIBLE,           /*!< ����ģʽ */
    CONTROL_MODE_FOLLOW,             /*!< ����ģʽ */
    CONTROL_MODE_RGB_MODE_OFF,       /*!< RGB�ر� */
    CONTROL_MODE_RGB_MODE_BREATHING, /*!< RGB������ */
    CONTROL_MODE_RGB_MODE_RUNNING,   /*!< RGB��ˮ�� */
    CONTROL_MODE_RGB_MODE_FLASHING,  /*!< RGB��˸ */
} control_mode_t;

/**
 * \brief   RGB�ʵ���ʾģʽ
 * \note    ����RGB�ʵ���ʾģʽ�жϺ��л�
 */
typedef enum {
    RGB_MODE_OFF,       /*!< �ʵƹر� */
    RGB_MODE_BREATHING, /*!< ������ģʽ */
    RGB_MODE_RUNNING,   /*!< ��ˮ��ģʽ */
    RGB_MODE_FLASHING,  /*!< ��˸ģʽ */
} rgb_mode_t;