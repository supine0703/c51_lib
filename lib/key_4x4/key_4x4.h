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
 *   - KEY_4X4_DELAY: 按键检测消抖函数, 不定义则不消抖
 *   - KEY_4X4_PIN: 连接矩阵键盘的引脚: 列, 右往左 0-3; 行, 下往上 4-7
 *   - KEY_PIN_END: 定义Value结束时 PIN 的终值, 不定义则为 0x0f
 * 
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作为妥协可以定义宏: 
 *   - UNUSED_KEY_4X4: 这将不会编译 Key_4x4 相关函数
*/

/* ========================================================================== */

#ifndef KEY_PIN_END
#define KEY_PIN_END 0x0f
#endif

/**
 * value 的返回值是 3-2位: 行号, 1-0位: 列号, ((row<<2) | column)
 * 即 返回 0x00-0x0a, 如果想扩大行列占位, 可以在在 'key_4x4.c' 文件中修改
*/
unsigned char KEY_4X4_Value(void);

#endif // KEY_4X4_H
