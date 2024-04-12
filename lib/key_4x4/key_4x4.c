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
#include "key_4x4/key_4x4.h"

#ifndef UNUSED_KEY_4X4

/* ========================================================================== */

#ifndef KEY_4X4_DELAY
#define KEY_4X4_DELAY
#endif // KEY_4X4_DELAY

#define DELAY KEY_4X4_DELAY
#define PIN KEY_4X4_PIN

unsigned char SwitchValue(unsigned char v)
{
    switch (v)
    {
    case 0x07:
    case 0x70:
        return 0;
    case 0x0b:
    case 0xb0:
        return 1;
    case 0x0d:
    case 0xd0:
        return 2;
    case 0x0e:
    case 0xe0:
        return 3;
    default:
        return 0xff;
    }
}

unsigned char KEY_4X4_Value(void)
{
    unsigned char column, row;
    PIN = 0x0f; // 列检测
    if (PIN != 0x0f)
    {
        column = PIN;
        DELAY; // 按键消抖
        if (column == PIN)
        {
            PIN = 0xf0; // 行检测
            if ((PIN != 0xf0) && ((row = PIN) == PIN))
            {
                row = SwitchValue(row) << 2;
                column = SwitchValue(column);
                return (row | column);
            }
        }
    }
    PIN = KEY_PIN_END;
    return 0xff;
}

/* ========================================================================== */

#endif // UNUSED_KEY_4X4
