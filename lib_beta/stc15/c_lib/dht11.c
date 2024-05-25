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

#ifndef UNUSED_DHT11

/* ========================================================================== */

sbit DQ = DHT11_DQ;

/**
 * @return: 1-存在; 0-不存在;
*/
static bit check(void)
{
    bit x;
    DQ = 1; // 复位DQ
    DQ = 0;
    delay_1ms(18); // 拉低大于 18ms
    DQ = 1;
    delay_5us(8); // 20~40us, 80us之内
    x = !DQ;      // 接收响应低电平
    delay_5us(8); // DHT11总共拉低 80us
    return x;     // 后拉高 80us
}

/**
 * @return: 期待 DQ 的值是 v, 如果超时未响应 则返回 0, 否则 DQ == v 返回 1
*/
static bit wait_DQ(bit v, u8 t)
{
    while (DQ != v && --t)
        delay_5us(1);
    return t != 0;
}

static bit read_byte(u8* byte)
{ // 拉低 50us, 拉高 0: 26~28us, 1: 70us; 拉低(next bit)
    u8 tmp;

    if (!wait_DQ(0, 40))
        return 0; // 等待进入低电平, 超过 200us 退出

    tmp = 0;
    for_c(8)
    {
        if (!wait_DQ(1, 20))
            return 0; // 等待50us低电平过去, 超过 100us 退出
        delay_5us(9); // 延时45us，如果还为高则数据为1，否则为0
        tmp <<= 1;
        if (DQ) // 数据为1时，使dat加1来接收数据1
        {
            tmp |= 1;
            if (!wait_DQ(0, 20))
                return 0; // 等待电平拉低, 超过 100us 退出
        }
    }

    if (byte)
        *byte = tmp;
    return 1;
}

/* ========================================================================== */

// DHT11通电后需要先放置 2s 等待内部启动, 立即读取不会应答
bit dht11_read(float* r, float* t)
{
    u8 chkSum, r_h, t_h, r_l, t_l;

    if (!check())
        return 0;

    delay_5us(16); // 拉高80us后, 开始接收数据

    if (!read_byte(&r_h))
        return 0; // 接收湿度高八位
    if (!read_byte(&r_l))
        return 0; // 接收湿度低八位
    if (!read_byte(&t_h))
        return 0; // 接收温度高八位
    if (!read_byte(&t_l))
        return 0; // 接收温度低八位
    if (!read_byte(&chkSum))
        return 0; // 接收校正和

    DQ = 1; // 结束

    if ((r_h + t_h + r_l + t_l) == chkSum) // 最后一字节为校验位，校验是否正确
    {
        // 这数据存储的真怪异
        if (r)
            *r = r_h + r_l * 0.1;
        if (t)
            *t = t_h + t_l * 0.1;
    }
    return 1;
}

/* ========================================================================== */

#endif // UNUSED_DHT11
