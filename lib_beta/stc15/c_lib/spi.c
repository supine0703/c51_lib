
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

#include "spi.h"

#ifndef UNUSED_SPI

// SPI初始化: 主要将SPI总线 从默认P1口上调制P4口, 然后初始化SPI总线
void spi_init(void)
{
#if 0
    P4M0 &= 0xFD;               // 配置 P4.1 MISO口 仅为输入功能
    P4M1 |= 0x02;
#endif
    AUXR1 |= 0X08; //将 SPI 调整到 P4.0 P4.1 P4.3
    SPDAT = 0;
    SPSTAT = 0xc0; //SPDAT.7和SPDAT.6写11，可以将中断标志清零。注意是写1才清零
    /**
     * SSIG 1 开启主机模式
     * SPEN 1 SPI使能
     * DORD 0 先发最高位
     * MSTR 1 主机模式
     * CPOL 0 SPICLK空闲时为低
     * CPHA 0 数据在SPICLK的前时钟沿驱动  时钟CPU_CLK/4
    */
    SPCTL = 0xd0;
}

// SPI为全双工通讯  所以在发送的同时可以接收到数据
u8 spi_send_byte(u8 byte)
{
    SPDAT = byte; // 将数据 写入

    while ((SPSTAT & 0x80) == 0)
        ; //等待写入完成

    SPSTAT = 0xc0; // 清除中断标志，和写冲突标志，注意是对应位写1才能清零

    return SPDAT; // 返回得到的数据
}

// SPI时钟速率设置
void spi_speed(u8 speed)
{
    // 每一次降速 都要先清为最高 在进行降速
    SPCTL &= 0xFC;
    SPCTL |= speed & 0x03;
    /**
     * SysClk/4,   SPR1=0, SPR0=0
     * SysClk/16,  SPR1=0, SPR0=1
     * SysClk/64,  SPR1=1, SPR0=0
     * SysClk/128, SPR1=1, SPR0=1
    */
}

/* ========================================================================== */

#endif // UNUSED_SPI
