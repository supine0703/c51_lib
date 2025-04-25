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

bit AT24C_Call(u16 addr)
{
    I2C_Start();
    I2C_Transmit(0xa0);
    if (I2C_CheckAck())
    {
        I2C_Transmit(addr >> 8);
        if (I2C_CheckAck())
        {
            I2C_Transmit(addr & 0xff);
            if (I2C_CheckAck())
            {
                return 1;
            }
        }
    }
    return 0;
}

void AT24C_Read(u16 addr, u8* dat, u8 len)
{
    if (AT24C_Call(addr))
    {
        I2C_Start(); // 重新启动I2C总线
        I2C_Transmit(0xa1);
        if (I2C_CheckAck())
        {
            FOR_C(len - 1)
            {
                *dat++ = I2C_Receive();
                I2C_Ack(1);
            }
            *dat = I2C_Receive();
            I2C_Ack(0);
        }
    }
    I2C_Stop();
}

// 页写入时序 最多为 页写缓冲器的大小 字节 超出会被循环覆盖 且此函数不带延迟
void AT24C_Write(u16 addr, u8* dat, u8 len)
{
    if (AT24C_Call(addr))
    {
        FOR_C(len)
        {
            I2C_Transmit(*dat);
            dat++;
            if (!I2C_CheckAck())
                break;
        }
    }
    I2C_Stop();
    Delay1ms(10);
}
