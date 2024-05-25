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

#include "i2c.h"

#ifndef UNUSED_AT24C

/* ========================================================================== */

static bit transmit_byte(u8 byte)
{
    i2c_transmit(byte);
    return i2c_check_ack();
}

static bit check(u8 device)
{ // device: 用于在多个at24c中选择
    i2c_start();
    if (transmit_byte(0xa0 | (device & 0x0f)))
        return 1;
    i2c_stop();
    return 0;
}

static bit inquiry(u16 addr)
{ // at24c512: 地址访问需要高低两字节
    return (check(0) && transmit_byte(addr >> 8) && transmit_byte(addr & 0xff));
}

void at24c_read(u16 addr, u8* dat, u8 len)
{
    if (len == 0)
        return;
    if (inquiry(addr))
    {
        if (check(1)) // 重新启动I2C总线
        {
            for_c(len - 1)
            {
                *dat++ = i2c_receive();
                i2c_ack(1);
            }
            *dat = i2c_receive();
            i2c_ack(0);
        }
        i2c_stop();
    }
}

// 页写入时序 最多为 页写缓冲器的大小 字节 超出会被循环覆盖
void at24c_write(u16 addr, u8* dat, u8 len)
{ // at24c512, 一页应该是 128 byte
    if (len == 0)
        return;
    if (inquiry(addr))
    {
        for_c(len)
        {
            if (!transmit_byte(*dat++))
                break;
        }
        i2c_stop();
        delay_1ms(10); // 至少 5ms
    }
}

/* ========================================================================== */

#endif // UNUSED_AT24C
