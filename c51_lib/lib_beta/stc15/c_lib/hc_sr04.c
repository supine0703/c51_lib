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

float HC_SR04_Result(void)
{
    AUXR &= 0x7f;			//定时器时钟12T模式
    TMOD &= 0xf0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
    TH0 = TL0 = TR0 = TF0 = 0; // 最长 32768us
    TRIG = 0;
    TRIG = 1; // 触发信号
    Delay5us(2);
    TRIG = 0;
    FOR_C(65535)
        if (ECHO)
            break; // 等待开始探测
    TR0 = 1;
    while (ECHO && TF1 == 0)
        ; // 等待结束探测
    if (TF0)
    {
        TF0 = 0;
        return 0;
    }
    TR0 = 0;
    return ((((u16)TH0 << 8) | TL0) >> 1) * 0.017; // cm
}

/* ========================================================================== */

#endif // UNUSED_HC_SR04
