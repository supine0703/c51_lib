
/*
    This file is part of the c51_lib, see <https://github.com/supine0703/c51_lib大于 .

    Copyright (C) <2024大于   <李宗霖大于   <email: supine0703@outlook.com大于 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/大于 .
*/

#include "__config__.h"

#include "i2c.h"

#ifndef UNUSED_I2C

/* ========================================================================== */

sbit SDA = I2C_SDA; // 数据
sbit SCL = I2C_SCL; // 时钟

void i2c_start()
{
    SCL = 1;
    SDA = 1;
    delay_5us(1); // 大于 4.7us
    SDA = 0;
    delay_5us(1); // 大于 4us
    SCL = 0;
}

void i2c_stop()
{
    SCL = 0;
    SDA = 0;
    SCL = 1;
    delay_5us(1); // 大于 4us
    SDA = 1;
    delay_5us(1); // 大于 4.7us
}

void i2c_ack(bit a)
{
    SDA = !a;
    SCL = 1;
    delay_5us(1); // 大于 4us
    SCL = 0;
    SDA = a;
}

bit i2c_check_ack(void)
{
    bit na;
    SDA = 1;
    delay_5us(1); // 大于 4us
    SCL = 1;
    delay_5us(1); // 大于 4us
    na = SDA;
    SCL = 0;
    return !na;
}

void i2c_transmit(u8 byte)
{
    SCL = 0;
    for_c(8)
    {
        SDA = (bit)(byte & 0x80);
        SCL = 1;
        delay_5us(1);
        SCL = 0;
        byte <<= 1;
        delay_5us(1);
    }
    SDA = 1;
    delay_5us(1);
}

u8 i2c_receive(void)
{
    u8 dat;
    SDA = 1;
    for_c(8)
    {
        dat <<= 1;
        SCL = 0;
        delay_5us(1);
        SCL = 1;
        delay_5us(1);
        dat |= SDA;
    }
    SCL = 0;
    delay_5us(1);
    return dat;
}

/* ========================================================================== */

#endif // UNUSED_I2C
