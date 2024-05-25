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

#include "u_type.h"
#include <STC12C5A60S2.H>

/* ========================================================================== */

extern void _nop_(void);
extern void Delay10us(u8 t);
extern void Delay1ms(u16 t);

#define LCD12864_DATA P0
#define LCD12864_RS  P3 ^ 5
#define LCD12864_RW  P3 ^ 6
#define LCD12864_EN  P3 ^ 4
#define LCD12864_PSB P3 ^ 7

#define HX711_DT  P1 ^ 1
#define HX711_SCK P1 ^ 0

#define KEY_4X4_PIN P2
#define KEY_4X4_DELAY Delay1ms(10);



#endif // __CONFIG___H
