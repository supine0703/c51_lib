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

extern void _nop_(void);

void Delay10us(u8 t)
{ // 平均误差: 0.37%
    u8 i;
    _nop_();
    while (t)
    {
        _nop_();
        _nop_();
        _nop_();
        i = t == 1 ? 18 : 23;
        while (--i);
        --t;
    }
}

void Delay1ms(u16 t)
{ // 平均误差: 0.2%
    u8 i, j;
    while (t)
    {
        for (i = 11; i; --i)
            for (j = 250; j; --j)
                ;
        --t;
    }
}

#if 0
// 无 nop 版本, 误差大, 但是我觉得节约不了多久, 30秒, 感觉不如加上 nop
void Delay10us(u8 t)
{ // 平均误差: t=1: 0.04%, t=100+: 1.25%
    u8 i;
    while (t)
    {
        i = t == 1 ? 19 : 24;
        while (--i);
        --t;
    }
}
#endif
