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

#ifndef KEY_4X4_H
#define KEY_4X4_H
/**
 * !注意:
 *   这是单按键识别, 请勿轻易尝试多按键同时按下, 如若造成任何损失, 自行承担.
 * 
 * 在使用之前需要在 '__config__.h' 中定义以下宏:
 *   - KEY_4X4_DELAY: 定义延迟函数调用宏, 按键检测消抖函数, 不定义则不消抖
 *   - KEY_4X4_PIN: 连接矩阵键盘的引脚: 列, 右往左 0-3; 行, 下往上 4-7
 * 
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作为妥协可以定义宏: 
 *   - UNUSED_KEY_4X4: 这将不会编译 Key_4x4 相关函数
*/

/* ========================================================================== */

#define KEY_NULL 0xff

/**
 * 注意: 如果外接矩阵键盘的引脚有其他外界设备, 如: 流水灯模块, 等
 *      首先是, 矩阵键盘肯定会影响其他模块运行的, 如果模块有输入也会影响按键矩阵
 *      所以, 建议是, 按键矩阵模块不要和其他硬件共用引脚
 *      但是, 如果不得不与没有输出的模块共用引脚, 如流水灯模块
 *      那么, 按键矩阵的大概率会从 低电平同化, 变为高电平同化
 *      例如, PIN=0x0f; 按下第一个按键: 低电平同化: 0x07; 高电平同化: 0x8f
 *      并且, 行列检测会置换: 低电平同化: 0x0f 行检测; 高电平同化: 0x0f 列检测
 * 
 * 结论: 我封装了一个两种情况兼容的 KEY_4X4_Value 函数, 不必考虑是哪种情况
 *      代价是, 其中的switch分支更复杂, 代码增加了一丢丢
 *      但是我在 'key_4x4.c' 文件中, 仍然分别保留了 低电平同化 和 高电平同化 两种不同的封装
 *      更详细的请查看 'key_4x4.c' 文件
 * 
 * @return: KEY_NULL(0xff) 即 没有被按下, 被按下则返回 0-15
*/
unsigned char KEY_4X4_Value(void);

/* ========================================================================== */

#endif // KEY_4X4_H
