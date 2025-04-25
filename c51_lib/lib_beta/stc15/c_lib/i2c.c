
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

#include "i2c.h"
#include "__config__.h"

sbit SDA = I2C_SDA; // 数据
sbit SCL = I2C_SCL; // 时钟

void I2C_Start()
{
    SCL = 1;
    SDA = 1;
    Delay5us(1); // >4.7us
    SDA = 0;
    Delay5us(1); // >4us
    SCL = 0;
}

void I2C_Stop()
{
    SCL = 0;
    SDA = 0;
    SCL = 1;
    Delay5us(1); // >4us
    SDA = 1;
    Delay5us(1); // >4.7us
}

void I2C_Ack(bit a)
{
    SDA = !a;
    SCL = 1;
    Delay5us(1); // >4us
    SCL = 0;
    SDA = a;
}

bit I2C_CheckAck(void)
{
    bit na;
    SDA = 1;
    Delay5us(1); // >4us
    SCL = 1;
    Delay5us(1); // >4us
    na = SDA;
    SCL = 0;
    return !na;
}

void I2C_Transmit(u8 byte)
{
    SCL = 0;
    FOR_C(8)
    {
        SDA = (bit)(byte & 0x80);
        SCL = 1;
        Delay5us(1);
        SCL = 0;
        byte <<= 1;
        Delay5us(1);
    }
    SDA = 1;
    Delay5us(1);
}

u8 I2C_Receive(void)
{
    u8 dat;
    SDA = 1;
    FOR_C(8)
    {
        dat <<= 1;
        SCL = 0;
        Delay5us(1);
        SCL = 1;
        Delay5us(1);
        dat |= SDA;
    }
    SCL = 0;
    Delay5us(1);
    return dat;
}

