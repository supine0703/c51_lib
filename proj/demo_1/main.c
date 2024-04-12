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
#include "delay/delay_c52.h"
#include "key_4x4/key_4x4.h"
#include "lcd1602/lcd1602.h"
#include "hc_sr04/hc_sr04.h"

// c语言标准库函数
extern int sprintf (char *, const char *, ...);

void LCD1602_ShowString(unsigned char* s)
{
    while (*s)
    {
        LCD1602_Write(*s++);
    }
}

void main(void)
{
    unsigned char num[10];

    LCD1602_Cmd(Set_8bit_2line_5x7);   // 命令6
    LCD1602_Cmd(Show_CursorOn);        // 命令4
    LCD1602_Cmd(Mode_CursorRightMove); // 命令3
    LCD1602_Cmd(Clear_Screen);         // 命令1
    LCD1602_Write('A');
    Delay1ms(1);

    while (1)
    {
        LCD1602_Cmd(Move_Cursor_Row1_Col(3));
        // 调用超声波传感器获取距离(单位: mm)
        sprintf(num, "%dmm", HC_SR04_Millimeter()); 
        // 显示在 lcd 屏幕上
        LCD1602_ShowString(num);
        Delay1ms(1000);
    }
}


