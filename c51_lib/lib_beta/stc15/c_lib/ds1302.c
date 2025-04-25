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

sbit SCK = DS1302_SCK; //时钟
sbit SDA = DS1302_SDA; //数据
sbit CE = DS1302_CE;   // DS1302复位

extern void _nop_(void);

// 秒分时日月周年
u8 code addr[] = { 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c };

void DS1302_WriteByte(u8 byte)
{
    FOR_C(8)
    {
        SCK = 0;
        SDA = byte & 0x01;
        byte >>= 1;
        SCK = 1;
    }
}

void DS1320_Write(u8 addr, u8 dat)
{
    CE = 0;
    _nop_();
    SCK = 0;
    _nop_();
    CE = 1; //启动
    _nop_();
    DS1302_WriteByte(addr); //发送地址
    DS1302_WriteByte(dat);  //发送数据
    CE = 0;                 //恢复
}

u8 DS1320_ReadBCD(u8 addr)
{
    u8 dat = 0x00;
    CE = 0;
    _nop_();
    _nop_();
    SCK = 0;
    _nop_();
    _nop_();
    CE = 1;
    _nop_();
    _nop_();

    DS1302_WriteByte(addr);

    FOR_C(8) //循环8次 读取数据
    {
        if (SDA)
            dat |= 0x80; //每次传输低字节
        SCK = 0;
        dat >>= 1; //右移一位
        _nop_();
        _nop_();
        _nop_();
        SCK = 1;
    }

    CE = 0;
    _nop_(); // 以下为DS1302复位的稳定时间
    _nop_();
    CE = 0;
    SCK = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SCK = 1;
    _nop_();
    _nop_();
    SDA = 0;
    _nop_();
    _nop_();
    SDA = 1;
    _nop_();
    _nop_();

    return (dat / 16 * 10) + (dat & 0x0f); //返回
}

//主要设置时钟芯片里的 秒分时日月周年
void DS1320_SetRTC(u8* set) //设定 日历
{
    u8 i;
    DS1320_Write(0x8E, 0x00); //写使能
    for (i = 0; i < 8; ++i)
    {
        DS1320_Write(addr[i], ((set[i] / 10 * 16) + (set[i] % 10)));
    }
    DS1320_Write(0x8E, 0x80); //写禁止
}
