
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

#ifndef UNUSED_STEP_MOTOR

/* ========================================================================== */

#define _SET(P, B, V) (P &= ~(0x0f << (B)), P |= (V) << (B))
#define SET(P, V)     _SET(P, V)

void step_motor_run(bit s, u16 size)
{ // 0: 顺时针, 1: 逆时针
    static u8 code spm_turn[] = {
        0x02,
        0x06,
        0x04,
        0x0c,
        0x08,
        0x09,
        0x01,
        0x03,
    };
    s8 i;

    for_c(size)
    {
        if (s)
        {
            for (i = 0; i < 8; ++i)
            {
                SET(STEP_MOTOR, spm_turn[i]);
                delay_5us(255);
            }
        }
        else
        {
            for (i = 7; i >= 0; --i)
            {
                SET(STEP_MOTOR, spm_turn[i]);
                delay_5us(255);
            }
        }
    }
}

/* ========================================================================== */

#endif // UNUSED_STEP_MOTOR
