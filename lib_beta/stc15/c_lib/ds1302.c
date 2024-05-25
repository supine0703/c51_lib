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

#ifndef UNUSED_DS1302

/* ========================================================================== */

#define MULTI_CLOCK 0xbe
#define SECOND      0x80 // 秒
#define MINUTE      0x82 // 分
#define HOUR        0x84 // 时
#define DAY         0x86 // 日
#define MONTH       0x88 // 月
#define WEEK        0x8a // 星期
#define YEAR        0x8c // 年
#define CONTROL     0x8e // 控制 WP: 0x00: 写允许, 0x80: 写禁止
#define CHARGE      0x90 // 充电

#define SINGLE_RAM 0xc0
#define MULTI_RAM  0xfe

#define RD 1
#define WT 0

#define WAIT _nop_, _nop_

/* ========================================================================== */

sbit SCK = DS1302_SCK; // 时钟
sbit SDA = DS1302_SDA; // 数据
sbit CE  = DS1302_CE;  // DS1302 使能(复位)

#if 0
// 秒分时日月周年
code u8 addr[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
#endif

static void start(void)
{
    CE = 0;
    WAIT; // at least: 2v: 200ns; 5v: 50ns
    SCK = 0;
    WAIT;
    CE = 1; // 启动
    WAIT;
}

static void write_byte(u8 byte)
{
    for_c(8)
    {
        SCK = 0;
        SDA = byte & 0x01;
        byte >>= 1;
        SCK = 1; // 上升沿写入
    }
}

static u8 read_byte(void)
{
    u8 byte = 0;
    for_c(8)
    {
        SCK = 0; // 下降沿读取
        byte >>= 1;
        if (SDA)
            byte |= 0x80; // 每次传输低字节
        SCK = 1;
    }
    return byte;
}

/* ========================================================================== */

// 如果不对SDA初始化, 第一次读取多字节会出现混乱
void ds1302_init(void)
{
    start();
    CE = 0;
    WAIT;
    SDA = 0; // 如果不对SDA初始化, 第一次读取多字节会出现混乱
}

// 7个字节: 秒分时日月周年
void ds1302_get(u8* get)
{
    u8 i;
    start();
    write_byte(MULTI_CLOCK | RD);
    for (i = 0; i < 7; ++i)
    {
        get[i] = read_byte();
    }
    read_byte(); // 读取写使能

    for (i = 0; i < 7; ++i)
    {
        get[i] = (get[i] / 16 * 10) + (get[i] & 0x0f);
    }
#if 1
    CE = 0;
    WAIT;
    SDA = 0;
#else
    // 如果有上拉电阻 可以不需要下面复位
    CE = 0;
    WAIT; // 以下为DS1302复位的稳定时间
    CE  = 0;
    SCK = 0;
    WAIT;
    WAIT;
    SCK = 1;
    WAIT;
    SDA = 0;
    WAIT;
    SDA = 1;
#endif
}

// 7个字节: 秒分时日月周年
void ds1302_set(u8* set)
{
    u8 i;
    start();
    write_byte(CONTROL | WT);
    write_byte(0x00); // 写使能
    CE = 0;

    start();
    write_byte(MULTI_CLOCK | WT);
    for (i = 0; i < 7; ++i)
    { // 不要用 for_c, 因为 write_byte 用了
        write_byte((set[i] / 10 * 16) + (set[i] % 10));
    }
    write_byte(0x80); // 写禁止
    CE = 0;
}

#if 0 // 单字节操作(大部分时候直接多字节操作更方便)
u8 DS1302_ReadBCD(u8 addr)
{
    u8 dat = 0x00;
    start();

    write_byte(addr | RD);

    dat = read_byte();

    CE = 0;
#if 1
    // 如果有上拉电阻 可以不需要下面复位
    WAIT; // 以下为DS1302复位的稳定时间
    CE  = 0;
    SCK = 0;
    WAIT;
    WAIT;
    SCK = 1;
    WAIT;
    SDA = 0;
    WAIT;
    SDA = 1;
#endif

    return (dat / 16 * 10) + (dat & 0x0f); // 返回
}
void DS1302_Write(u8 addr, u8 dat) 
{ 
    start();
    write_byte(addr); // 发送地址
    write_byte(dat);  // 发送数据
    CE = 0;                 // 恢复
}
// 主要设置时钟芯片里的 秒分时日月周年
void ds1302_setRTC(u8* set) // 设定 日历
{
    u8 addr = SECOND | WT;
    DS1302_Write(CONTROL, 0x00); // 写使能
    while (addr < CONTROL)
    {
        DS1302_Write(addr, ((*set) / 10 * 16) + ((*set) % 10));
        addr += 2;
        set++;
    }
    DS1302_Write(CONTROL, 0x80); // 写禁止
}
#endif

#if 0
// 返回每月天数, 包含计算闰年
u8 month_days(u16 year, u8 month)
{
    u8 code days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 0 || month > 12)
        return 0;
    if (month == 2 && ((year & 3) == 0))
        return 29;
    return days[month - 1];
}

// 2001年1月1日 星球一 1-7[周一至周日]
u8 what_day(u16 year, u8 month, u8 day)
{
    u8 curr  = 1;
    u16 diff = year - 2001;
    diff += (diff >> 2) + (diff > 0x7fff); // %7 : 1月1日 -1

    while (curr != month)
    {
        diff += MonthDays(year, curr);
        curr++;
    } // month月1日 -1

    diff += day - 1;
    return (diff % 7) + 1;
}
#endif 

/* ========================================================================== */

#endif // UNUSED_DS1302
