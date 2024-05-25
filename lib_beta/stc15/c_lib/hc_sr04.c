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

#include "__config__.h"

#ifndef UNUSED_HC_SR04

/* ========================================================================== */

sbit TRIG = HC_SR04_TRIG;
sbit ECHO = HC_SR04_ECHO;

float hc_sr04_result(void)
{
    AUXR &= 0x7f;              // 定时器时钟12T模式
    TMOD &= 0xf0;              // 设置定时器模式
    TMOD |= 0x01;              // 设置定时器模式
    TH0 = TL0 = TR0 = TF0 = 0; // 最长 32768us

    TRIG = 0;
    TRIG = 1;     // 触发信号
    delay_5us(2); // 拉高 10us
    TRIG = 0;
#if 0
    while (!ECHO)
        ;
    TR0 = 1;
    while (ECHO)
        ;
    TR0 = 0;
    return ((((u16)TH0 << 8) | TL0) >> 1) * 0.017; // cm
#else
    for_c(65535)
    { // 防止没有超声波传感器卡死
        if (ECHO)
        { // 等待开始探测
            TR0 = 1;
            while (ECHO && TF0 == 0)
                ; // 等待结束探测
            if (TF0 == 1)
            {
                return TF0 = TR0 = 0;
            }
            TF0 = TR0 = 0;
            return ((((u16)TH0 << 8) | TL0) >> 1) * 0.017; // cm
        }
    }
    return 0;
#endif
}

/* ========================================================================== */

#endif // UNUSED_HC_SR04
