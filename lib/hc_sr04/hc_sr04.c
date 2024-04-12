/*
    This file is part of the c51_lib, see <https://github.com/supine0703/c51_lib>.

    Copyright (C) <2024>  <李宗霖>  <email: supine0703@outlook.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "hc_sr04/hc_sr04.h"
#include "__config__.h"

#ifndef UNUSED_HC_SR04

/* ========================================================================== */

sbit TRIG = HC_SR04_DEFINE_TRIG;
sbit ECHO = HC_SR04_DEFINE_ECHO;

#if HC_SR04_TIMER == 0

#define TIMER_INIT                                                             \
    TMOD &= 0xf0;                                                              \
    TMOD |= 0x01;                                                              \
    TH0 = TL0 = TR0 = 0;
#define TIMER_START (TR0 = 1)
#define TIMER_STOP (TR0 = 0)
#define TIME_VALUE (((unsigned int)TH0 << 8) | TL0)

#elif HC_SR04_TIMER == 1

#define TIMER_INIT                                                             \
    TMOD &= 0x0f;                                                              \
    TMOD |= 0x10;                                                              \
    TH1 = TL1 = TR1 = 0;
#define TIMER_START (TR1 = 1)
#define TIMER_STOP (TR1 = 0)
#define TIME_VALUE (((unsigned int)TH1 << 8) | TL1)

#else
#error INVALID HC_SR04_TIMER
#endif

/**
 * 计算方法: 340m/s(来回路程距离) / 2 * TIME_VALUE
 * 换算后: 0.17 mm/us(单程距离) * Tus(0<=T<=65535)
 * 最大可计算 约: 650cm, HC_SR04 
 * 探测距离: 2cm-450cm, 足够了
 * 返回值: us * mm/us = mm
*/
unsigned int HC_SR04_Millimeter(void)
{
    TIMER_INIT;
    TRIG = 0;
    TRIG = 1; // 触发信号
    HC_SR04_DELAY_10US;
    TRIG = 0;
    while (!ECHO)
        ; // 等待开始探测
    TIMER_START;
    while (ECHO)
        ; // 等待结束探测
    TIMER_STOP;
    return (unsigned int)(TIME_VALUE * 0.17);
}

/* ========================================================================== */

#endif // UNUSED_HC_SR04
