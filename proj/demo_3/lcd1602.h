#ifndef LCD1602_H
#define LCD1602_H

void LCD1602_Cmd(unsigned char cmd);

void LCD1602_Write(unsigned char dat);

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
#define Move_CGROM_ADDRESS(X) ((0x00 <= (X) && (X) <= 0x1f) ? 0x40 | (X) : 0)
#define Move_Cursor_Row1_Col(X) ((0x00 <= (X) && (X) <= 0x27) ? 0x80 | (X) : 0)
#define Move_Cursor_Row2_Col(X) ((0x00 <= (X) && (X) <= 0x27) ? 0xc0 | (X) : 0)

#endif // LCD1602_H