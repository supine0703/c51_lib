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

#include "ds18b20/ds18b20.h"
#include "__config__.h"

#ifndef UNUSED_DS18B20

#define SEARCH_ROM 0xf0
#define READ_ROM 0x33
#define MATCH_ROM 0x55
#define SKIP_ROM 0xcc
#define ALARM_SEARCH 0xec

#define CONVERT_T 0x44
#define WRITE_SCRATCHPAD 0x4e
#define READ_SCRATCHPAD 0xbe
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2ROM 0xb8
#define READ_POWER_SUPPLY 0xb4

/* ========================================================================== */

sbit DQ = DS18B20_DEFINE_DQ;

/**
 * @return: 0-存在; 1-不存在;
*/
bit DS18B20_InitCheck(void)
{
    bit x;
    DQ = 1; // 复位DQ
    DQ = 0;
    DS18B20_DELAY_10US_FN(60); // 拉低 480~960us
    DQ = 1;
    DS18B20_DELAY_10US_FN(12); // 等待 15~60us 240us之内
    x = DQ;                    // 总线60~240us低电平
    DQ = 1;                    // 释放总线
    DS18B20_DELAY_10US_FN(24); // 保证时序完整 480us * 2
    return x;
} // 约 bit 973 uchar 976 机器周期

unsigned char DS18B20_ReadByte(void)
{
    unsigned char i = 8;   // 读 8b 数据
    unsigned char dat = 0; // 存储读数据初始化 0
    do
    {           // 串行读8位数据，先读低位后读高位
        DQ = 0; // 拉低
        DS18B20_DELAY_10US_FN(1);
        DQ = 1; // 15μs内拉高释放总线
        dat >>= 1;
        if (DQ)
            dat |= 0x80;
        DS18B20_DELAY_10US_FN(5); // 每个读时段 最少60us
    } while (--i);
    return dat;
} // 约 587 机器周期

void DS18B20_WriteByte(unsigned char dat)
{
    unsigned char i = 8; // 写 8b 数据
    do
    {           // 串行写8位数据，先写低位后写高位
        DQ = 0; // 拉低
        DS18B20_DELAY_10US_FN(1); // 至少间隔1us 低于15us
        DQ = dat & 0x01;          // 写 '1' 在15μs内拉高
        DS18B20_DELAY_10US_FN(5); // 写 '0' 拉低60μs 10+50
        DQ = 1;
        dat >>= 1;
    } while (--i);
} // 约 584 机器周期

/* ========================================================================== */

void DS18B20_Convert() // 温度转换
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(CONVERT_T);
} // 约 2151 机器周期

int DS18B20_ReadTemp() // 温度读取
{
    unsigned char low, high;
    if (DS18B20_InitCheck())
        return DS18B20_ERROR_T;

    low = 0;
    high = 0;
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(READ_SCRATCHPAD);
    low = DS18B20_ReadByte();
    high = DS18B20_ReadByte();
    return ((((int)high) << 8) | low);
} // 约 3663 机器周期

/* ========================================================================== */

/**
 * @param upperLimit: 温度上限
 * @param lowerLimit: 温度下限
 * @param resolution: 分辨率(精度)
 *                      00:  9位, 93.75 ms
 *                      01: 10位, 187.5 ms
 *                      10: 11位,   375 ms
 *                      11: 12位,   750 ms
*/
void DS18B20_Set(char upperLimit, char lowerLimit, unsigned char resolution)
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(WRITE_SCRATCHPAD); // 写暂存器指令4E
    DS18B20_WriteByte(upperLimit);       // 写高速缓存器TH高温限值
    DS18B20_WriteByte(lowerLimit);       // 写高速缓存器TL低温限值
    DS18B20_WriteByte((resolution << 5) | 0x1f); // 精度设置
}

/**
 * @param upperLimit: 温度上限
 * @param lowerLimit: 温度下限
 * @param resolution: 分辨率(精度)
 * @param temperature: 温度
*/
void DS18B20_Load(
    char* upperLimit,
    char* lowerLimit,
    unsigned char* resolution,
    int* temperature
)
{
    if (temperature)
        *temperature = DS18B20_ReadTemp();
    else if (DS18B20_InitCheck())
        return;
    else
    {
        DS18B20_WriteByte(SKIP_ROM);
        DS18B20_WriteByte(READ_SCRATCHPAD);
        DS18B20_ReadByte();
        DS18B20_ReadByte();
    }

    if (upperLimit)
        *upperLimit = DS18B20_ReadByte();
    else
        DS18B20_ReadByte();
    if (lowerLimit)
        *lowerLimit = DS18B20_ReadByte();
    else
        DS18B20_ReadByte();
    if (resolution)
    {
        *resolution = DS18B20_ReadByte();
        *resolution >>= 5;
    }
}

void DS18B20_Save(void)
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(COPY_SCRATCHPAD);
    DS18B20_DELAY_10US_FN(12); // 需要给DS18B20存储的时间
    // 经测试 最好高于80us 否则可能会造成存入失败或者数据错误
}

#if 0
// 暂时废弃 

void DS18B20_Update(void)
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0xb8);
    while (!DQ)
    {
    }
}

char DS18B20_Mode(void)
{
    char dat;
    if (DS18B20_InitCheck())
        return -1;
    DS18B20_WriteByte(0xcc);
    DS18B20_WriteByte(0xb4);
    DQ = 0; // 拉低
    DS18B20_DELAY_10US_FN(1);
    DQ = 1; // 15μs内拉高释放总线
    if (DQ)
        dat = 1;
    else dat = 0;
    DS18B20_DELAY_10US_FN(5); // 最少60us
    return dat;
}

#endif

/* ========================================================================== */

#endif // UNUSED_DS18B20
