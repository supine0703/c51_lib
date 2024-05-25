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

#ifndef UNUSED_DS18B20

/* ========================================================================== */

#define SEARCH_ROM   0xf0
#define READ_ROM     0x33
#define MATCH_ROM    0x55
#define SKIP_ROM     0xcc
#define ALARM_SEARCH 0xec

#define CONVERT_T         0x44
#define WRITE_SCRATCHPAD  0x4e
#define READ_SCRATCHPAD   0xbe
#define COPY_SCRATCHPAD   0x48
#define RECALL_E2ROM      0xb8
#define READ_POWER_SUPPLY 0xb4

/* ========================================================================== */

sbit DQ = DS18B20_DQ;

static void reset(void)
{
    DQ = 1; // 复位DQ
    DQ = 0;
    delay_5us(120); // 拉低 480~960us
    DQ = 1;
}

/**
 * @return: 1-存在; 0-不存在;
*/
static bit check(void)
{
    bit x;
    reset();
    delay_5us(24); // 等待 15~60us 240us之内
    x  = !DQ;      // 总线60~240us低电平
    DQ = 1;        // 释放总线
    delay_5us(48); // 保证时序完整 480us * 2
    return x;
}

static u8 read_byte(void)
{
    u8 byte = 0; // 存储读数据初始化 0
    for_c(8)
    {           // 串行读8位数据，先读低位后读高位
        DQ = 0; // 拉低
        delay_5us(1);
        DQ = 1; // 15μs内拉高释放总线
        byte >>= 1;
        if (DQ)
            byte |= 0x80;
        delay_5us(9); // 每个读时段 最少60us
    }
    return byte;
}

static void write_byte(u8 byte)
{
    for_c(8)
    {                     // 串行写8位数据，先写低位后写高位
        DQ = 0;           // 拉低
        delay_5us(1);     // 至少间隔1us 低于15us
        DQ = byte & 0x01; // 写 '1' 在15μs内拉高
        delay_5us(9);     // 写 '0' 拉低60μs 10+50
        DQ = 1;
        byte >>= 1;
    }
}

/* ========================================================================== */

void ds18b20_convert() // 温度转换
{
    if (!check())
        return;
    write_byte(SKIP_ROM);
    write_byte(CONVERT_T);
}

float ds18b20_read_temp() // 温度读取
{
    s16 n;
    if (!check())
        return 0xffff;

    write_byte(SKIP_ROM);
    write_byte(READ_SCRATCHPAD);
    n = read_byte();
    n |= ((s16)read_byte()) << 8;
    reset(); // 结束复位
    return n * 0.0625;
}

/* ========================================================================== */

/**
 * @param upper_limit: 温度上限
 * @param lower_limit: 温度下限
 * @param accuracy: 精度(分辨率)
*/
void ds18b20_get(u8* upper_limit, u8* lower_limit, u8* accuracy)
{
    u8 tmp;
    if (!check())
        return;

    write_byte(SKIP_ROM);
    write_byte(READ_SCRATCHPAD);
    read_byte();
    read_byte();

    tmp = read_byte();
    if (upper_limit)
        *upper_limit = tmp;

    tmp = read_byte();
    if (lower_limit)
        *lower_limit = tmp;

    tmp = read_byte();
    if (accuracy)
        *accuracy = tmp >> 5;
    
    reset(); // 结束复位
}

/**
 * @brief: 只是设置到 RAM, 存储需要通过 save, 写入 EEPROM
 * @param upperLimit: 温度上限
 * @param lowerLimit: 温度下限
 * @param resolution: 分辨率(精度)
 *                      00:  9位, 93.75 ms
 *                      01: 10位, 187.5 ms
 *                      10: 11位,   375 ms
 *                      11: 12位,   750 ms
*/
void ds18b20_set(u8 upperLimit, u8 lowerLimit, u8 resolution)
{
    if (!check())
        return;
    write_byte(SKIP_ROM);
    write_byte(WRITE_SCRATCHPAD); // 写暂存器指令4E
    write_byte(upperLimit);       // 写高速缓存器TH高温限值
    write_byte(lowerLimit);       // 写高速缓存器TL低温限值
    write_byte(((resolution & 0x03) << 5) | 0x1f); // 精度设置
}

void ds18b20_save(void)
{
    if (check())
        return;
    write_byte(SKIP_ROM);
    write_byte(COPY_SCRATCHPAD);
    delay_5us(24); // 需要给DS18B20存储的时间
    // 经测试 最好高于80us 否则可能会造成存入失败或者数据错误
}

/* ========================================================================== */

#if 0
// 暂时废弃 

void DS18B20_Update(void)
{
    if (check())
        return;
    write_byte(0xcc);
    write_byte(0xb8);
    while (!DQ)
    {
    }
}

char DS18B20_Mode(void)
{
    char dat;
    if (check())
        return -1;
    write_byte(0xcc);
    write_byte(0xb4);
    DQ = 0; // 拉低
    delay_5us(1);
    DQ = 1; // 15μs内拉高释放总线
    if (DQ)
        dat = 1;
    else dat = 0;
    delay_5us(5); // 最少60us
    return dat;
}

#endif

/* ========================================================================== */

#endif // UNUSED_DS18B20
