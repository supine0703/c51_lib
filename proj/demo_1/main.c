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
#include "ds18b20/ds18b20.h"
#include "hc_sr04/hc_sr04.h"
#include "key_4x4/key_4x4.h"
#include "lcd1602/lcd1602.h"

// c语言标准库函数
extern int sprintf(char*, const char*, ...);

void LCD1602_ShowString(unsigned char* s)
{
    while (*s)
    {
        LCD1602_Write(*s++);
    }
}

#if 1

void main(void)
{
    LCD1602_Cmd(Clear_Screen);         // 命令1
    LCD1602_Cmd(Mode_CursorRightMove); // 命令3
    LCD1602_Cmd(Show_CursorOn);        // 命令4
    LCD1602_Cmd(Set_8bit_2line_5x7);   // 命令6
    LCD1602_ShowString("Hello World!");
    while (1)
    {
    }
}

#elif 1

unsigned int code arr[] = {337, 675, 1350, 2700};
unsigned int convertCount = 0;
unsigned char sel = 3;
bit convert_finished = 0;

void init(void)
{
    TMOD = 0x02; // 定时器0 方式2
    TH0 = 0x00;  // 自动装填
    TL0 = 0x00;  // 记 256 次

    EA = 1;
    ET0 = 1;
}

void main(void)
{
    unsigned char line1[] = "T:          R:  ";
    unsigned char line2[] = "H:      L:      ";
    unsigned char t[9];
    unsigned char r[4];
    unsigned char h[5];
    unsigned char l[5];
    char rr, hh, ll;

    t[7] = 0xdf;
    t[8] = 0x43;

    // 从 DS18B20 读取 温度上下限 分辨率
    DS18B20_Load(&hh, &ll, &rr, 0);
    DS18B20_Load(&hh, &ll, &rr, 0);
    // sel = rr;

    init();

    DS18B20_Convert();
    TR0 = 1;

    LCD1602_Cmd(Clear_Screen);         // 命令1
    LCD1602_Cmd(Mode_CursorRightMove); // 命令3
    LCD1602_Cmd(Show_CursorOn);        // 命令4
    LCD1602_Cmd(Set_8bit_2line_5x7);   // 命令6
    LCD1602_ShowString(line1);
    LCD1602_Cmd(Move_Cursor_Row2_Col(0));
    LCD1602_ShowString(line2);
    LCD1602_Cmd(Move_Cursor_Row1_Col(8));
    LCD1602_ShowString(t + 7);

    sprintf(h, "%d", (int)hh);
    sprintf(l, "%d", (int)ll);
    sprintf(r, "%d", (int)rr);

    LCD1602_Cmd(Move_Cursor_Row1_Col(14));
    LCD1602_ShowString(r);
    LCD1602_Cmd(Move_Cursor_Row2_Col(2));
    LCD1602_ShowString(h);
    LCD1602_Cmd(Move_Cursor_Row2_Col(10));
    LCD1602_ShowString(l);

    while (1)
    {
        if (convert_finished)
        {
            sprintf(t, "%6.2f", DS18B20_ReadTemp() * 0.0625);
            DS18B20_Convert();
            convert_finished = 0;
            TR0 = 1;
            LCD1602_Cmd(Move_Cursor_Row1_Col(2));
            LCD1602_ShowString(t);
        }
    }
}

void int_t0(void) interrupt 1
{
    if (!convert_finished && ++convertCount >= arr[sel])
    {
        convertCount = 0;
        convert_finished = 1;
        TR0 = 0;
        TH0 = TL0 = 0x00;
    }
}

#elif 1
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
#endif