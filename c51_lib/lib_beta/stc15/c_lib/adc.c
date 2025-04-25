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

// 定义 ADC_CONTR 寄存器位取值
#define ADC_POWER 0x80 // ADC power control bit
#define ADC_FLAG 0x10  // ADC complete flag 模数转换结束标志位
// ADC start control bit  模数转换启动控制位
// 每当手动将其置 1 后, AD转换开始, 当AD转换结束后这个位就会自动置 0
#define ADC_START 0x08 

// 转换速度控制位SPEED0和SPEED1，共四种状态，对应四种转换速度
#define ADC_SPEED_0 0x00 // 540 clocks
#define ADC_SPEED_1 0x20 // 360 clocks
#define ADC_SPEED_2 0x40 // 180 clocks
#define ADC_SPEED_3 0x60 // 90 clocks


/**
 * @param ch: 0-7 选择 P10-P17
*/
u16 ADC_Result(u8 ch)
{
    u16 Vo;
    P1ASF = (1 << ch); // 选择P1口的哪一口 这里的口和 ch 要对应才能达到选择该口
    // 开启ADC,采集 P1^ch 引脚的值
    ADC_CONTR = ADC_POWER | ADC_SPEED_0 | ADC_START | ch;
    // 这么用语句的主要原因就是不能位寻址
    // 通道选择在后3位所以直接用一个整数表示ch

    // 设置 ADC_CONTR 寄存器后需加4个CPU时钟周期的延时，才能保证值被写入 ADC_CONTR 寄存器
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    while (!(ADC_CONTR & ADC_FLAG))
        ; //检测是否ADC完成

    ADC_CONTR &= ~ADC_FLAG; // 转换标志位: 手动将其置0 等待下次硬件置1

    /**
     * @param CLK_DIV: 第5位控制存储模式
     * 0: ADC_RES(高8位) + ADC_RESL(低2位)
     * 1: ADC_RESL(高2位) + ADC_RES(低8位)
    */
    Vo = ADC_RES;
    Vo = (Vo << 2) | ADC_RESL;
    return Vo;
}
