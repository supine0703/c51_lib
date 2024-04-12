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

#include "lcd1602/lcd1602.h"
#include "__config__.h"

#ifndef UNUSED_LCD1602

/* ========================================================================== */

sbit RS = LCD1602_DEFINE_RS; // 寄存器选择
sbit RW = LCD1602_DEFINE_RW; // 读/写
sbit EN = LCD1602_DEFINE_EN; // 使能
#define DT LCD1602_DATA

#ifdef LCD1602_TIMEOUT_BREAK
bit lcd1602_timeout;
#define CHECK_TYPE bit
#define CHECK_BUSY                                                             \
    if (!LCD1602_CheckBusy())                                                  \
    return
#if 1
#define START_CHECK
#define STOP_CHECK return !lcd1602_timeout;
#else
/**
 * 下面这种定义会在每一次运行 LCD1602 相关代码时都重置 lcd1602_timeout
 * 这意味着, 如果没有连接 LCD1602 每一次死循环都需要手动处理
 * 我认为上面的定义更合理, 代价是手动置1, 需要手动归0
*/
#define START_CHECK lcd1602_timeout = 0;
#define STOP_CHECK                                                             \
    lcd1602_timeout = 1;                                                       \
    return !lcd1602_timeout;
#endif
#define AND_NOT_TIMEOUT &&(!lcd1602_timeout)
#else
#define CHECK_TYPE void
#define CHECK_BUSY LCD1602_CheckBusy()
#define START_CHECK
#define STOP_CHECK
#define AND_NOT_TIMEOUT
#endif

/* ========================================================================== */

CHECK_TYPE LCD1602_CheckBusy(void)
{
    unsigned char busy;
    START_CHECK;
    DT = 0xff;
    do
    {
        EN = 0;
        RS = 0;
        RW = 1;
        EN = 1;
        busy = DT;
    } while ((busy & 0x80) AND_NOT_TIMEOUT);
    EN = 0;
    STOP_CHECK;
}

extern void _nop_(void);

void LCD1602_WriteByte(bit rs, unsigned char byte)
{
    CHECK_BUSY;
    RW = 0;
    RS = rs; // 0 写命令  1 写数据
    DT = byte;
    EN = 1; // EN: 1 -> 0
    _nop_();
    EN = 0;
}

/* ========================================================================== */

void LCD1602_Cmd(unsigned char cmd)
{
    if (cmd == 0)
        return;
    LCD1602_WriteByte(0, cmd);
}

void LCD1602_Write(unsigned char dat)
{
    LCD1602_WriteByte(1, dat);
}

#ifdef LCD1602_NEED_READ_DATA
unsigned char LCD1602_Read(void)
{
    unsigned char dat;
    CHECK_BUSY;
    DT = 0xff; // 如果是准双向IO口, 在做输入时要先拉高
    RW = 1;
    RS = 1;
    EN = 1;
    dat = DT;
    EN = 0;
    return dat;
}
#endif

/* ========================================================================== */

#endif // UNUSED_LCD1602
