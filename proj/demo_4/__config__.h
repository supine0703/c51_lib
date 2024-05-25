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

#ifndef __CONFIG___H
#define __CONFIG___H

#include <STC12C5A60S2.H>

/* ========================================================================== */

/**
 * 注意: 以下定义中, 存在两个容易出错的概念:
 *   - 定义函数调用宏: 这里需要的是调用后函数 例如 'Delay(1)'
 *   - 定义函数宏: 这里只用函数名就可以了 例如 'Delay', 说明再源文件中会调用函数并传参
 *               通常宏会有_FN作为后缀, 例如: 'DS18B20_DELAY_10US_FN'
*/

/* -------- define for delay -------- */

#if 1
#define UNUSED_DELAY_10US       // 将不编译 Delay10us
#endif
#if 1
#define UNUSED_DELAY_1MS        // 将不编译 Delay1ms
#endif
#if 1
#define DELAY_USE_HZ 11059200   // 不定义将不使用, 仅支持 11059200, 12000000
#endif

/* ---------------------------------- */
/* ------- define for lcd1602 ------- */

#if 0
#define UNUSED_LCD1602              // 将不编译 LCD1602 相关函数
#endif
#if 0
#define LCD1602_TIMEOUT_BREAK       // 设置可手动跳出查忙
#endif
#if 0
#define LCD1602_NEED_READ_DATA      // 需要编译 LCD1602_Read
#endif
#define LCD1602_DATA P0             // 数据 to LCD1602
#define LCD1602_DEFINE_RS P3 ^ 5    // 寄存器选择
#define LCD1602_DEFINE_RW P3 ^ 6    // 读/写
#define LCD1602_DEFINE_EN P3 ^ 4    // 使能

/* ---------------------------------- */
/* --- define for matrix keyboard --- */

#if 1
#define UNUSED_KEY_4X4              // 将不编译 KEY_4X4 相关函数
#endif
#if 1
extern void Delay1ms(unsigned int t);
#define KEY_4X4_DELAY Delay1ms(10)  // 定义延迟函数调用宏, 不定义则不消抖s
#endif
#define KEY_4X4_PIN P2

// -------------------------------------
/* --- define for stepping motor ---- */

#if 1
#define UNUSED_STEPPING_MOTOR   // 将不编译 STEPPING_MOTOR 相关函数
#endif

// -------------------------------------

/* ========================================================================== */

#endif // __CONFIG___H
