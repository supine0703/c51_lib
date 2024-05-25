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

#ifndef DELAY_C52_H
#define DELAY_C52_H
/**
 * 在使用之前需要在 '__config__.h' 中定义以下宏:
 *   - DELAY_USE_HZ: c52单片机的晶振频率, 目前仅支持: 11059200, 12000000
 *
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作为妥协可以定义宏: 
 *   - UNUSED_DELAY_10US: 这将不会编译 Delay10us 函数
 *   - UNUSED_DELAY_1MS:  这将不会编译 Delay1ms  函数
*/

/* ========================================================================== */

/**
 * @param t: 需要延迟的时间
 *   Delay10us: 0-255; 
 *   Delay1ms: 0-65535;
 * 
 * 因为 10us 太短, 为了减少误差, '0' 为特殊取值, 等价于 256, 
 * 1ms 则正常, 仅包含调用耗时, 分别是 11(11.0592M), 17(12M) 个机器周期
*/

void Delay10us(unsigned char t);

void Delay1ms(unsigned int t);

/* ========================================================================== */

#endif // DELAY_C52_H
