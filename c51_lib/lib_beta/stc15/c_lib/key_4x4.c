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

#ifndef UNUSED_KEY_4X4

/* ========================================================================== */

#define PIN KEY_4X4_PIN

u8 SwitchValue(u8 v)
{
    switch (v)
    {
    case 0x0e:
    case 0xe0: return 0x00;  
    case 0x0d: return 0x01;
    case 0xd0: return 0x04;
    case 0x0b: return 0x02;
    case 0xb0: return 0x08;
    case 0x07: return 0x03;
    case 0x70: return 0x0c;
    default:
        return 0xff;
    }
}

u8 KEY_4X4_Value(void)
{
    u8 n;
    PIN = 0x0f; // 列检测
    if (PIN != 0x0f)
    {
        n = PIN;
        KEY_4X4_DELAY; // 按键消抖
        if (n == PIN)
        {
            PIN = 0xf0; // 行检测
            n = SwitchValue(n);
            return (SwitchValue(PIN) | n);
        }
    }
    return 0xff;
}

/* ========================================================================== */

#endif // UNUSED_KEY_4X4
