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

#ifndef LCD1602_H
#define LCD1602_H
/**
 * 在使用之前需要在 '__config__.h' 中定义以下宏:
 *   - LCD1602_DEFINE_RS: 寄存器选择引脚
 *   - LCD1602_DEFINE_RW: 读/写引脚
 *   - LCD1602_DEFINE_EN: 使能引脚
 *   - LCD1602_DATA: LCD1602 与 单片机连接的 数据引脚s
 * 
 * 如果需要使用从 LCD1602 上读取数据, 需要定义宏:
 *   - LCD1602_NEED_READ_DATA: 这将会编译 LCD1602_Read 函数
 * 
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作为妥协可以定义宏: 
 *   - UNUSED_LCD1602: 这将不会编译 LCD1602 相关函数
*/

/* ========================================================================== */

void LCD1602_Cmd(unsigned char cmd);    // 写入命令 命令1-8

void LCD1602_Write(unsigned char dat);  // 写入数据 命令10

#ifdef LCD1602_NEED_READ_DATA
unsigned char LCD1602_Read(void);       // 读数据 命令11
#endif

/**
 * 为了解决单片机: 无连接或者连接的lcd1602故障, 导致查忙一直无响应的死循环的问题
 * 可以在 '__config__.h' 中定义宏: LCD1602_TIMEOUT_BREAK
 * 便可以自行通过定时器中断或者外部中断将 'lcd1602_timeout' 置零 来跳出查忙
 * 注意: 如果手动将 lcd1602_timeout 置于1 请手动归0, 否则查忙将不可用
 * 更详细的内容请查看 'lcd1602.c' 文件
*/
#ifdef LCD1602_TIMEOUT_BREAK
extern bit lcd1602_timeout;
#endif

/* ========================================================================== */

// 0x00 是无效指令

// 命令1：清屏
#define Clear_Screen            0x01    // ... ...

// 命令2：光标返回0位
#define Return_Cursor           0x02    // 0x02-0x03

// 命令3：输入字符后 屏幕/光标 移动
#define Mode_ScreenLeftMove     0x05    // 屏幕左移
#define Mode_ScreenRightMove    0x07    // 屏幕右移
#define Mode_CursorLeftMove     0x04    // 光标自减
#define Mode_CursorRightMove    0x06    // 光标自增

// 命令4：控制屏幕和光标状态
#define Show_ScreenOff          0x08    // 屏幕关闭 0x08-0x0b
#define Show_CursorOff          0x0c    // 亮屏无光标 0x0c 0x0d
#define Show_CursorOn           0x0e    // 亮屏有光标
#define Show_CursorFlicker      0x0f    // 亮屏光标闪烁

// 命令5：光标/屏幕 左/右移
/**
 * 值得注意的是 屏幕不会真的移动，屏幕移动是靠 DDRAM 整体的循环移动呈现的
 */
#define Shift_CursorLeft        0x10    // 光标左移 0x10-0x13
#define Shift_CursorRight       0x14    // 光标右移 0x14-0x17
#define Shift_ScreenLeft        0x1c    // 屏幕左移 0x1c-0x1f
#define Shift_ScreenRight       0x18    // 屏幕右移 0x18-0x1b

// 命令6：设置数据位 显示行数 点阵
#define Set_4bit_1line_5x7      0x20    // 0x20-0x23
#define Set_4bit_1line_5x10     0x24    // 0x24-0x27
#define Set_4bit_2line_5x7      0x28    // 0x28-0x2b
#define Set_4bit_2line_5x10     0x2c    // 0x2c-0x2f
#define Set_8bit_1line_5x7      0x30    // 0x30-0x33
#define Set_8bit_1line_5x10     0x34    // 0x34-0x37
#define Set_8bit_2line_5x7      0x38    // 0x38-0x3b
#define Set_8bit_2line_5x10     0x3c    // 0x3c-0x3f

// 命令7：设置字库地址 0x40-0x5f
// 命令8：设置光标位置 0x80-0xa7  0xc0-0xe7
#if 1
#define Move_CGROM_ADDRESS(X)   (0x40 | (X))
#define Move_Cursor_Row1_Col(X) (0x80 | (X))
#define Move_Cursor_Row2_Col(X) (0xc0 | (X))
#else 
// 上面的宏定义方式会检查是否合法 但是会使得代码膨胀 如果熟练运用LCD1602的指令 使用下面的定有方式更好
#define Move_CGROM_ADDRESS(X) ((0x00 <= (X) && (X) <= 0x1f) ? 0x40 | (X) : 0)
#define Move_Cursor_Row1_Col(X) ((0x00 <= (X) && (X) <= 0x27) ? 0x80 | (X) : 0)
#define Move_Cursor_Row2_Col(X) ((0x00 <= (X) && (X) <= 0x27) ? 0xc0 | (X) : 0)
#endif

/* ========================================================================== */

/**
 * 以下是 LCD1602 的命令字简略汇总: RS RW D7~D0
 * 1. 清屏               00 0x01
 * 2. 光标返回            00 0x02-0x03
 * 3. 模式设置            00 0x04-0x07
 * 4. 显示设置            00 0x08-0x0f
 * 5. 光标/字符移位        00 0x10-0x1f
 * 6. 功能设置            00 0x20-0x3f
 * 7. CGROM地址设置       00 0x40-0x5f
 * 8. DDRAM地址设置       00 0x80-0xa7 0xc0-0xe7 (0xa7-0xbf 0xe7-0xff无用)
 * 9. 读忙/DDRAM地址      01 0x00-0xff (第8位: 忙标志, 第1-7位: DDRAM地址)
 * 10. 写入数据           10 0x00-0xff (后面为读取的数据)
 * 11. 读取数据           11 0x00-0xff (后面为写入的数据)
*/

#endif // LCD1602_H
