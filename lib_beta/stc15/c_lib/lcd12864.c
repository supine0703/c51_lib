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

#define DT LCD_DATA
sbit RS = LCD_RS;
sbit RW = LCD_RW;
sbit EN = LCD_EN;

// 申明为静态的, 可以简化源文件内命名, 不会被其他文件引用
static void write(bit rs, u8 byte)
{
    EN = 0;
    RS = rs;
    RW = 0;
    DT = byte;
    EN = 1;
    EN = 0;
    delay_5us(15); // 至少72us
}

/* ========================================================================== */

void lcd_cmd(u8 cmd)
{
    write(0, cmd);
    if (cmd == 0x01)
    {
        delay_5us(200);
        delay_5us(105); // clear need 1.6ms
    }
}

void lcd_show(u8 dat)
{
    write(1, dat);
}

/* ========================================================================== */

#ifndef LCD_PRINT_BUFF_SIZE
#define LCD_PRINT_BUFF_SIZE 65
#endif

#include <stdarg.h>
extern int vsprintf(char*, const char*, char*);
void lcd_printf(const char* format, ...)
{
    u8 xdata s_buf[LCD_PRINT_BUFF_SIZE];
    u8 xdata* s;
    va_list arg; // 定义可变参数列表数据类型的变量arg

    va_start(arg, format); // 从format开始，接收参数列表到arg变量
    vsprintf(s_buf, format, arg); // 打印格式化字符串和参数列表到字符数组中
    va_end(arg);                  // 结束变量arg

    s = s_buf;
    while (*s)
        lcd_show(*s++);
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
