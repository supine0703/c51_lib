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

#ifndef DS18B20_H
#define DS18B20_H
/**
 * 在使用之前需要在 '__config__.h' 中定义以下宏:
 *   - DS18B20_DELAY_10US_FN: 时序延迟函数函数, 请尽可能精确 否则容易出错
 *   - DS18B20_DEFINE_DQ: DS18B20 的单总线
 * 
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作为妥协可以定义宏: 
 *   - UNUSED_DS18B20: 这将不会编译 DS18B20 相关函数
 *   - UNUSED_DS18B20_SETTINGS: 这件不会编译 DS18B20 功能设置相关的函数
 * 
 * 注意: 因为 DS18B20 是时序敏感的器件, 所以在调用相关函数之前请先关闭中断,
 *      否则, 中断可能会影响 DS18B20 的使用, 读取出现混乱
*/

/* ========================================================================== */

/**
 * 当前库封装的全为单机操作, 多机操作看是否有精力研究吧
 * 个人觉得: DS18B20的多机操作太笨重了, 效率很低, 不如多端口操作 ds18b20, 
 *         端口不够就添加个单片机当从机管理, 这样感觉更好一些
*/

#define DS18B20_ERROR_T 0xffff

/**
 * 使用方法: 调用开始转换后, 等待转换文成后调用 DS18B20_ReadTemp 读取温度
 * 
 * 分辨率: 最大转换时间 默认是 12位
 *   00:  9位, 93.75 ms
 *   01: 10位, 187.5 ms
 *   10: 11位,   375 ms
 *   11: 12位,   750 ms
 * 转换后需要等待转换完成后才能读取正确的温度
 * 
 * 在 11.0592MHz 对于定时次数, 256 * { 337, 675, 1350, 2700 }
 * 
*/
void DS18B20_Convert(); // 温度转换

/**
 * @return: 读取到的温度, 但是需要 t*0.0625 或者 t/16 或者 (t>>4).((t&0x0f)*625)
*/
int DS18B20_ReadTemp(); // 温度读取

/* ========================================================================== */

/**
 * 以下时功能设置相关的函数, 如果定义 UNUSED_DS18B20_SETTINGS , 将不会被编译
*/

/**
 * @param upperLimit: 温度上限
 * @param lowerLimit: 温度下限
 * @param resolution: 分辨率(精度)
*/
void DS18B20_Set(char upperLimit, char lowerLimit, unsigned char resolution);

/**
 * @param upperLimit: 温度上限
 * @param lowerLimit: 温度下限
 * @param resolution: 分辨率(精度)
*/
void DS18B20_Load(
    char* upperLimit,
    char* lowerLimit,
    unsigned char* resolution,
    int* temperature
);

void DS18B20_Save(void); // 将当前设置存储至 E2ROM

/* ========================================================================== */

#endif // DS18B20_H
