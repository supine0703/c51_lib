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
#include "function.h"

u8 buff[33];
u8 key;
u8 t_count = 0;
float w0 = 0, w, k = 1.0;

void init(void)
{
    AUXR &= 0x7F; // 定时器时钟12T模式
    TMOD &= 0xF0; // 设置定时器模式
    TL0 = 0x00;   // 设置定时初始值
    TH0 = 0x4C;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时

    EA = 1;
    ET0 = 1;
}

void main(void)
{
    init();
    LCD12864_Init();
    LCD12864_String("Hello");

    while (1)
    {
        if ((key = KEY_4X4_Value()) != 0xff)
        {
            LCD12864_Cmd(0x88);
            sprintf(buff, "%X", (u16)key);
            LCD12864_String(buff);
            while (KEY_4X4_Value() != 0xff)
                ;
            switch (key)
            {
            case 0:
                w0 = HX711_Read();
                break;
            case 1:
                w = HX711_Read();
                k = 550 / (w - w0);
                break;
            default:
                break;
            }
        }
        if (t_count > 20)
        {
            t_count = 0;
            w = HX711_Read();
            w = (w - w0) * k;
            if (w < 0)
                w = -w;
            LCD12864_Cmd(0x01);
            sprintf(buff, "%07.2f", w);
            LCD12864_Cmd(0x80);
            LCD12864_String(buff);
        }
    }
}

void int_t0() interrupt 1
{
    t_count++;
}
