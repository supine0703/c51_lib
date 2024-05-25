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

#include "key_4x4/key_4x4.h"
#include "__config__.h"

#ifndef UNUSED_KEY_4X4

/* ========================================================================== */

#ifndef KEY_4X4_DELAY
#define KEY_4X4_DELAY
#endif // KEY_4X4_DELAY

#define PIN KEY_4X4_PIN

#if 1

unsigned char SwitchValue(unsigned char v)
{
    switch (v)
    {
    case 0x07: case 0x8f:
    case 0x70: case 0xf8:
        return 0;
    case 0x0b: case 0xf4:
        return 1;
    case 0xb0: case 0x4f:
        return 4;
    case 0x0d: case 0xf2:
        return 2;
    case 0xd0: case 0x2f:
        return 8;
    case 0x0e: case 0xf1:
        return 3;
    case 0xe0: case 0x1f:
        return 12;
    default:
        return KEY_NULL;
    }
}

unsigned char KEY_4X4_Value(void)
{
    unsigned char n;
    PIN = 0x0f; // 第一次检测
    if (PIN != 0x0f)
    {
        n = PIN;
        KEY_4X4_DELAY; // 按键消抖
        if (n == PIN)
        {
            PIN = 0xf0; // 第二次检测
            return (SwitchValue(n) | SwitchValue(PIN));
        }
    }
    return KEY_NULL;
}

#else

// 可以通过定义 KEY_PIN_BE_USED 宏, 切换为 高电平同化版本的
#ifndef KEY_PIN_BE_USED

unsigned char SwitchValue(unsigned char v)
{
    switch (v)
    {
    case 0x07:
    case 0x70:
        return 0x00;
    case 0x0b:
    case 0xb0:
        return 0x01;
    case 0x0d:
    case 0xd0:
        return 0x02;
    case 0x0e:
    case 0xe0:
        return 0x03;
    default:
        return KEY_NULL;
    }
}

unsigned char KEY_4X4_Value(void)
{
    unsigned char column, row;
    PIN = 0x0f; // 列检测
    if (PIN != 0x0f)
    {
        column = PIN;
        KEY_4X4_DELAY; // 按键消抖
        if (column == PIN)
        {
            PIN = 0xf0; // 行检测
            return ((SwitchValue(PIN) << 2) | SwitchValue(column));
        }
    }
    return KEY_NULL;
}

#else

unsigned char SwitchValue(unsigned char v)
{
    switch (v)
    {
    case 0x8f:
    case 0xf8:
        return 0x00;
    case 0x4f:
    case 0xf4:
        return 0x01;
    case 0x2f:
    case 0xf2:
        return 0x02;
    case 0x1f:
    case 0xf1:
        return 0x03;
    default:
        return KEY_NULL;
    }
}

unsigned char KEY_4X4_Value(void)
{
    unsigned char column, row;
    PIN = 0xf0; // 列检测
    if (PIN != 0xf0)
    {
        column = PIN;
        KEY_4X4_DELAY; // 按键消抖
        if (column == PIN)
        {
            PIN = 0x0f; // 行检测
            return ((SwitchValue(PIN) << 2) | SwitchValue(column));
        }
    }
    return KEY_NULL;
}

#endif // KEY_PIN_BE_USED

#endif

/* ========================================================================== */

#endif // UNUSED_KEY_4X4
