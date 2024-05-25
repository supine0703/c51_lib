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

#ifndef UNUSED_LCD12864

/* ========================================================================== */

#define DT LCD12864_DATA
sbit RS = LCD12864_RS;
sbit RW = LCD12864_RW;
sbit EN = LCD12864_EN;

void LCD12864_Write(bit rs, u8 byte)
{
    EN = 0;
    RS = rs;
    RW = 0;
    EN = 0;
    DT = byte;
    EN = 1;
    EN = 0;
    Delay10us(8);
}

void LCD12864_Cmd(u8 cmd)
{
    LCD12864_Write(0, cmd);
    if (cmd == 0x01)
        Delay10us(152); // clear need 1.6ms
}

void LCD12864_Show(u8 dat)
{
    LCD12864_Write(1, dat);
}

void LCD12864_Init(void)
{
    LCD12864_Cmd(0x01);
    LCD12864_Cmd(0x06); // 默认 06 可以不要
    LCD12864_Cmd(0x0e);
    LCD12864_Cmd(0x30);
}

void LCD12864_String(u8* s)
{
    while (*s)
    {
        LCD12864_Show(*s++);
    }
}

#if 0
// lcd12864 一直忙, 没有找到原因和解决方法, 采用延时方法吧
void LCD12864_Busy(void)
{
    do
    {
        DT = 0xff;
        EN = 0;
        RS = 0;
        RW = 1;
        EN = 1;
    } while (DT & 0x80);
    EN = 0;
}
#endif

/* ========================================================================== */

#endif // UNUSED_LCD12864

/**
 * 附录: lcd12864 指令表 (不全, 部分几乎完全不用的指令省了)

 * 0x01         清屏            待命模式
 * 0x02         光标返回
 * 0x03                        允许输入卷动地址
 * 0x04         模式光标左移      第一行反白
 * 0x05         模式文字右移      第二行反白
 * 0x06         模式光标右移      第三行反白
 * 0x07         模式文字左移      第四行反白
 * 0x08         屏幕关闭         进入睡眠模式
 * 0x0c         亮屏光标关闭      退出睡眠模式
 * 0x0e         亮屏光标开启
 * 0x0f         亮屏光标闪烁
 * 0x10         光标左移
 * 0x14         光标右移
 * 0x18         文字左移
 * 0x1c         文字右移
 * 0x30         普通指令         普通指令
 * 0x34         扩展指令         扩展指令
 * 0x36                        绘图模式打开
 * 0x40-0x7f                   卷动/IRAM地址
 * 0x80         第1行
 * 0x90         第2行
 * 0x88         第3行
 * 0x98         第4行
 * 0x80-0xff                   设定绘图RAM
*/
