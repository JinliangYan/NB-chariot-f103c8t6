/*
 * @Author: Jinliang miku.cy@foxmail.com
 * @Date: 2024-03-17 13:52:11
 * @LastEditors: Jinliang miku.cy@foxmail.com
 * @LastEditTime: 2024-03-17 14:13:55
 * @FilePath: \NB-chariot-f103c8t6\HARDWARE\TIM\timer.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TIMER_H
#define __TIMER_H	
#include "sys.h" 


void tim2_init(u32 arr,u32 psc);
void tim3_init(u32 arr,u32 psc);
void tim4_init(u32 arr,u32 psc);

	
#endif


