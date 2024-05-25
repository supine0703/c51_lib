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

#ifndef FUNCTION_H
#define FUNCTION_H

#include "u_type.h"

// c
extern int sprintf (char *, const char *, ...);

// delay
extern void Delay10us(u8 t);
extern void Delay1ms(u16 t);

// lcd12864
extern void LCD12864_Cmd(u8 cmd);
extern void LCD12864_Init(void);
extern void LCD12864_String(u8* s);


// key_4x4
extern u8 KEY_4X4_Value(void);

// hx711
extern float HX711_Read();

#endif // FUNCTION_H
