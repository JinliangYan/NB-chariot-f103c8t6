/*** 
 * @Author: Jinliang miku.cy@foxmail.com
 * @Date: 2024-03-14 20:40:51
 * @LastEditors: Jinliang miku.cy@foxmail.com
 * @LastEditTime: 2024-03-17 11:09:00
 * @FilePath: \NB-chariot-f103c8t6\USER\mode.h
 * @Description: 小车模式类型头文件
 * @
 * @Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
/**
 * \brief   小车模式集合
 * \note    用于小车模式判断和切换
 */
typedef enum {
    CONTROL_MODE_JOYSTICK,           /*!< 摇杆控制模式 */
    CONTROL_MODE_GRAVITY,            /*!< 陀螺仪控制模式 */
    CONTROL_MODE_EVADIBLE,           /*!< 避障模式 */
    CONTROL_MODE_FOLLOW,             /*!< 跟随模式 */
    CONTROL_MODE_RGB_MODE_OFF,       /*!< RGB关闭 */
    CONTROL_MODE_RGB_MODE_BREATHING, /*!< RGB呼吸灯 */
    CONTROL_MODE_RGB_MODE_RUNNING,   /*!< RGB流水灯 */
    CONTROL_MODE_RGB_MODE_FLASHING,  /*!< RGB闪烁 */
} control_mode_t;

/**
 * \brief   RGB彩灯显示模式
 * \note    用于RGB彩灯显示模式判断和切换
 */
typedef enum {
    RGB_MODE_OFF,       /*!< 彩灯关闭 */
    RGB_MODE_BREATHING, /*!< 呼吸灯模式 */
    RGB_MODE_RUNNING,   /*!< 流水灯模式 */
    RGB_MODE_FLASHING,  /*!< 闪烁模式 */
} rgb_mode_t;