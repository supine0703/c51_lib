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

#include "delay/delay_c52.h"
#include "__config__.h"

extern void _nop_(void);

#ifdef DELAY_USE_HZ

/* ========================================================================== */

/**
 * 因为 10us 太短, 为了减少误差, '0' 为特殊取值, 等价于 256 , 
 * 1ms 则正常, 仅包含调用耗时, 分别是 11, 17 个机器周期
*/

#if DELAY_USE_HZ == 11059200

/**
 * 11.0592MHz:
 *   Delay10us误差(值大):
 *     - 第一次: 8.5%
 *     - 之后的奇数次: 约4.16%
 *     - 之后的偶数次: 约3.08%
 *   Delay1ms误差(值大):
 *     - 第一次: 0.043%
 *     - 之后次: 约0.47%
*/
#ifndef UNUSED_DELAY_10US
void Delay10us(unsigned char t)
{
    unsigned char i;
    _nop_();
    if (!(--t))
        return;
    do
    {
        for (i = 2; i; --i)
            ;
        if (!(--t))
            break;
        _nop_();
        for (i = 3; i; --i)
            ;
    } while (--t);
}
#endif // UNUSED_DELAY_10US

#ifndef UNUSED_DELAY_1MS
void Delay1ms(unsigned int t)
{
    unsigned char i;
    while (t)
    {
        --t;
        for (i = (t ? 151 : 149); i; --i)
            ;
    }
}
#endif // UNUSED_DELAY_1MS

/* ========================================================================== */

#elif DELAY_USE_HZ == 12000000

#ifndef UNUSED_DELAY_10US
void Delay10us(unsigned char t)
{ // lcall mov ret : 5us
    unsigned char i;
    _nop_(); // nop : 1us
    if (!--t)
        return; // dec mov jz : 4us
    do          // 1+1+6+2 = 10us
    {
        _nop_(); // nop : 1us
        for (i = 3; i; --i)
            ;      // mov djnz : 3us
    } while (--t); // djnz : 2us
} // (t-1)*10us + (5+1+4)us
#endif // UNUSED_DELAY_10US

#ifndef UNUSED_DELAY_1MS
void Delay1ms(unsigned int t) // 12MHz
{                             // 软件延迟 参数用uchar 比uint 精准
    unsigned char i;
    while (t--) // mov dec mov jnz orl jz : 9us
    {
        i = 123;    // mov : 1us
        while (i--) // mov dec mov jz : 6us * 124
            ;       // jmp : 2us * 123
    }               // 每当低位为 0 会多1us处理高位(dec) 忽略
} // (9+1+8*124-2=1000)us * t + ((t/256)+10+6)us 约 t ms
#endif // UNUSED_DELAY_1MS

/* ========================================================================== */

#else
#error INVALID HZ
#endif // DELAY_USE_HZ ==

#endif // DELAY_USE_HZ
