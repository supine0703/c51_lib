# COPYRIGHT
```
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
```
# 目录
- [COPYRIGHT](#copyright)
- [目录](#目录)
- [使用说明](#使用说明)
- [第一步: 创建Keil5项目](#第一步-创建keil5项目)
- [第二步: 编写文件模板](#第二步-编写文件模板)
- [第三步: 根据要求编写 lib](#第三步-根据要求编写-lib)
  - [c语言标准库函数](#c语言标准库函数)
  - [DELAY](#delay)
  - [LCD1602](#lcd1602)
  - [DS18B20](#ds18b20)
  - [HC\_SR04](#hc_sr04)
  - [KEY\_4X4](#key_4x4)
  - [BUZZER](#buzzer)
  - [DC\_MOTOR](#dc_motor)
  - [STEP\_MOTOR](#step_motor)
- [第四步: 编译通过烧入程序](#第四步-编译通过烧入程序)


# 使用说明
***这是针对于竞赛准备纸质材料所准备的文档***\
大部分函数为简略或者阉割版, 详细参见完整 'c51_lib', 下方为简单的竞赛步骤, 重点在第三步写lib, 故其余的简单带过;\
比赛时注释可忽略, 这里注释是方方便查看

<div STYLE="page-break-after: always;"></div>


# 第一步: 创建Keil5项目
建议选择 AT89C52 或者 STC89C52RC


# 第二步: 编写文件模板
编写程序必备的 '\_\_config\_\_.h' 和 'main.c'文件\
'\_\_config\_\_.h' 用来统一管理项目需要的 引脚以及其他参数定义\
'main.c' 文件中 init 用来初始化单片机的初始状态, 中断等

```cpp 
// __config__.h
#ifndef __CONFIG___H
#define __CONFIG___H

#if 0 // 二选一, 一定不要都包含
#include <REG52.H> // AT89C52 用这个
#else
#include <STC89C5xRC.H> // STC89C52RC 用这个
#endif

// Add your settings

#endif // __CONFIG___H
```

```cpp 
// main.c
#include "__config__.h"
// Add other headers

void init(void)
{
}

void main(void)
{
  
    init();
    
    while (1)
    {
    }
}
```

<div STYLE="page-break-after: always;"></div>

# 第三步: 根据要求编写 lib
考虑到竞赛时, 现场编程实时性, 下面将会给出精简版的 c51_lib 库, 会减少许多复杂宏逻辑

## c语言标准库函数
```cpp
// c语言的标准格式化函数, 可以将数字等转为串再输出
extern int sprintf(char*, const char*, ...);
// %d: 整形 %f: 浮点型; %x: 16进制显示
// %d, %x: char需要强转(int)否则会出错;
```

<div STYLE="page-break-after: always;"></div>


## DELAY
创建 'delay.h' 和 'delay.c' 文件, 并封装, 这里以 c52 11.0592MHz 为例:

```cpp
// delay.h
#ifndef DELAY_C52_H
#define DELAY_C52_H

void Delay10us(unsigned char t); // 特殊 传入0 等价于 256

void Delay1ms(unsigned int t); // 0 正常

#endif // DELAY_C52_H
```

```cpp
// delay.c
#include "__config__.h"
#include "delay_c52.h"

extern void _nop_(void);

void Delay10us(unsigned char t)
{
    unsigned char i;
    _nop_();
    if (!(--t))
        return;
    do
    {
        for (i = 2; i; --i)
            ;
        if (!(--t))
            break;
        _nop_();
        for (i = 3; i; --i)
            ;
    } while (--t);
}

void Delay1ms(unsigned int t) 
{
    unsigned char i;
    while (t)
    {
        --t;
        for (i = (t ? 151 : 149); i; --i)
            ;
    }
}
```

<div STYLE="page-break-after: always;"></div>


## LCD1602
创建 'lcd1602.h' 和 'lcd1602.c' 文件, 并封装\
在 '\_\_config\_\_.h' 文件中添加宏定义数据口和相关引脚信息\
以 P0 数据口 RS: P10, RW: P11, E: P12 为例

```cpp
// __config__.h
#define LCD1602_DATA P0             // 数据口
#define LCD1602_DEFINE_RS P1 ^ 0    // 寄存器选择
#define LCD1602_DEFINE_RW P1 ^ 1    // 读/写
#define LCD1602_DEFINE_EN P1 ^ 2    // 使能
```

```cpp
// 常用封装
void LCD1602_ShowString(unsigned char* s)
{
    while (*s)
    {
        LCD1602_Write(*s++);
        // 如果需要逐字输出, 可以加延迟函数
    }
}

void LCD1602_Init(void)
{ // 根据需求初始化
    LCD1602_Cmd(Clear_Screen);
    LCD1602_Cmd(Mode_CursorRightMove);
    LCD1602_Cmd(Show_CursorOff);
    LCD1602_Cmd(Set_8bit_2line_5x7);
}
```

```cpp
// lcd1602.h
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
#define Move_CGROM_ADDRESS(X)   ((0x00 <= (X) && (X) <= 0x1f) ? 0x40 | (X) : 0)

// 命令8：设置光标位置 0x80-0xa7  0xc0-0xe7
#define Move_Cursor_Row1_Col(X) ((0x00 <= (X) && (X) <= 0x27) ? 0x80 | (X) : 0)
#define Move_Cursor_Row2_Col(X) ((0x00 <= (X) && (X) <= 0x27) ? 0xc0 | (X) : 0)

#endif // LCD1602_H
```

```cpp
// lcd1602.c
#include "lcd1602.h"
#include "__config__.h"

#define DT LCD1602_DATA
sbit RS = LCD1602_DEFINE_RS; // 寄存器选择
sbit RW = LCD1602_DEFINE_RW; // 读/写
sbit EN = LCD1602_DEFINE_EN; // 使能

void LCD1602_CheckBusy(void)
{
    unsigned char busy;
    DT = 0xff;
    do
    {
        EN = 0;
        RS = 0;
        RW = 1;
        EN = 1;
        busy = DT;
    } while (busy & 0x80);
    EN = 0;
}

extern void _nop_(void);

void LCD1602_WriteByte(bit rs, unsigned char byte)
{
    LCD1602_CheckBusy();
    RW = 0;
    RS = rs; // 0 写命令  1 写数据
    DT = byte;
    EN = 1; // EN: 1 -> 0
    _nop_();
    EN = 0;
}

void LCD1602_Cmd(unsigned char cmd)
{
    if (cmd == 0)
        return;
    LCD1602_WriteByte(0, cmd);
}

void LCD1602_Write(unsigned char dat)
{
    LCD1602_WriteByte(1, dat);
}
```

<div STYLE="page-break-after: always;"></div>


## DS18B20
创建 'ds18b20.h' 和 'ds18b20.c' 文件, 并封装\
在 '\_\_config\_\_.h' 文件中添加宏定义总线引脚和延迟函数\
以 P15 和 上面的 Delay10us函数 为例\
- 注意: 因为 DS18B20 是时序敏感的器件, 所以在调用相关函数之前请先关闭中断,
  否则, 中断可能会影响 DS18B20 的使用, 读取出现混乱

```cpp
// __config__.h
extern void Delay10us(unsigned char t);
#define DS18B20_DELAY_10US_FN Delay10us // 定义延迟函数函数宏, 必须要可传一个参数
#define DS18B20_DEFINE_DQ P1 ^ 5        // ds18b20 单总线
```

```cpp
// ds18b20.h
#ifndef DS18B20_H
#define DS18B20_H

#define DS18B20_ERROR_T 0xffff

/*
 * 分辨率: 最大转换时间 默认是 12位
 *   00:  9位, 93.75 ms
 *   01: 10位, 187.5 ms
 *   10: 11位,   375 ms
 *   11: 12位,   750 ms
 * 转换后需要等待转换完成后才能读取正确的温度
 * 
*/
void DS18B20_Convert(); // 温度转换

/**
 * @return: 读取到的温度, 但是值需要 t*0.0625
 * 注意: ° ASCII码: 0xdf;
*/
int DS18B20_ReadTemp(); // 温度读取

#endif // DS18B20_H
```

```cpp
// ds18b20.c
#include "ds18b20.h"
#include "__config__.h"

#define SKIP_ROM 0xcc
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xbe

#define WRITE_SCRATCHPAD 0x4e
#define COPY_SCRATCHPAD 0x48

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
}

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
}

void DS18B20_WriteByte(unsigned char dat)
{
    unsigned char i = 8; // 写 8b 数据
    do
    {           // 串行写8位数据，先写低位后写高位
        DQ = 0; // 拉低
        DS18B20_DELAY_10US_FN(1); // 至少间隔1us 低于15us
        DQ = dat & 0x01;          // 写 '1' 在15us内拉高
        DS18B20_DELAY_10US_FN(5); // 写 '0' 拉低60us 10+50
        DQ = 1;
        dat >>= 1;
    } while (--i);
}

/* ========================================================================== */

void DS18B20_Convert() // 温度转换
{
    if (DS18B20_InitCheck())
        return;
    DS18B20_WriteByte(SKIP_ROM);
    DS18B20_WriteByte(CONVERT_T);
}

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
} 
```

```cpp
// 使用方法, 两种:
// 通过延迟函数 或者中断, 下面延时延迟函数, 中断的就将中断函数的过程拿来做事, 定时换成中断
void main(void)
{
    init();
    LCD1602_Init();
    while (1)
    {
        DS18B20_Convert();
        Delay1ms(750);
        sprintf(str, "%6.2f", DS18B20_ReadTemp() * 0.0625);
        LCD1602_Cmd(Move_Cursor_Row1_Col(3));
        LCD1602_ShowString(str);
    }
}
```

<div STYLE="page-break-after: always;"></div>


## HC_SR04
创建 'hc_sr04.h' 和 'hc_sr04.c' 文件, 并封装\
在 '\_\_config\_\_.h' 文件中添加宏定义总线引脚和延迟函数\
以 TRIG:P35 ECHO:P35, T0 和 上面的 Delay10us函数 为例\
- 注意: 请勿打开所使用定时器的中断

```cpp
// __config__.h
extern void Delay10us(unsigned char t);
#define HC_SR04_DELAY_10US Delay10us(1) // 定义延迟函数调用宏 触发信号需保持 10us 以上
#define HC_SR04_DEFINE_TRIG P3 ^ 4      // 触发信号输入
#define HC_SR04_DEFINE_ECHO P3 ^ 5      // 回响信号输出
```

```cpp
// hc_sr04.h
#ifndef HC_SR04_H
#define HC_SR04_H

/**
 * 注意事项: 探测并返回, 整个过程不会用到中断, 请勿开启所使用的定时器中断
 * @return: 返回超声波传感器测量计算后的距离, 单位: mm (毫米)
*/
unsigned int HC_SR04_Millimeter(void);

#endif // HC_SR04_H
```

```cpp
// hc_sr04.c
// 下面的是使用定时器0的, 定时器1 将相应的修改即可
#include "hc_sr04.h"
#include "__config__.h"

sbit TRIG = HC_SR04_DEFINE_TRIG;
sbit ECHO = HC_SR04_DEFINE_ECHO;

unsigned int HC_SR04_Millimeter(void)
{
    unsigned int v;
    TMOD &= 0xf0;
    TMOD |= 0x01;
    TH0 = TL0 = TR0 = 0;
    TRIG = 0;
    TRIG = 1; // 触发信号
    HC_SR04_DELAY_10US;
    TRIG = 0;
    while (!ECHO)
        ; // 等待开始探测
    TR0 = 1;
    while (ECHO)
        ; // 等待结束探测
    TR0 = 0;

    v = ((unsigned int)TH0 << 8) | TL0;
    return ((v >> 2) - ((v / 25) << 1)); // 等价于 v*0.17 但是这样运行效率更高
}
```

```cpp
// 使用方法
sprintf(str, "%dmm", HC_SR04_Millimeter());
LCD1602_ShowString(str);
```

<div STYLE="page-break-after: always;"></div>


## KEY_4X4
矩阵键盘模块最好独占引脚, 详细原因下方 'key_4x4.h' 示例中有讲述\
创建 'key_4x4.h' 和 'key_4x4.c' 文件, 并封装\
在 '\_\_config\_\_.h' 文件中添加宏定义 矩阵键盘使用的引脚\
以 P2 和 上面的 Delay10us函数 为例

```cpp
// __config__.h
extern void Delay1ms(unsigned int t);
#define KEY_4X4_DELAY Delay1ms(10)  // 定义延迟函数调用宏, 消抖
#define KEY_4X4_PIN P2
```

```cpp
// key_4x4.h
#ifndef KEY_4X4_H
#define KEY_4X4_H

#define KEY_NULL 0xff

/**
 * 注意: 如果外接矩阵键盘的引脚有其他外界设备, 如: 流水灯模块, 等
 *      首先是, 矩阵键盘肯定会影响其他模块运行的, 如果模块有输入也会影响按键矩阵
 *      所以, 建议是, 按键矩阵模块不要和其他硬件公用引脚
 *      但是, 如果不得不与没有输出的模块公用引脚, 如流水灯模块
 *      那么, 按键矩阵的大概率会从 低电平同化, 变为高电平同化
 *      例如, PIN=0x0f; 按下第一个按键: 低电平同化: 0x07; 高电平同化: 0x8f
 *      并且, 行列检测会置换: 低电平同化: 0x0f 行检测; 高电平同化: 0x0f 列检测
 * 
 * @return: KEY_NULL(0xff) 即 没有被按下, 被按下则返回 0-15
*/
unsigned char KEY_4X4_Value(void);

#endif // KEY_4X4_H
```

```cpp
// key_4x4.c
#include "__config__.h"
#include "key_4x4.h"

#ifndef KEY_4X4_DELAY
#define KEY_4X4_DELAY
#endif // KEY_4X4_DELAY

#define DELAY KEY_4X4_DELAY
#define PIN KEY_4X4_PIN

unsigned char SwitchValue(unsigned char v)
{
    /**
     * 如果使用的引脚没有外接其他模块, case 第一列,
     * 如果外接了例如, 流水灯模块, 大概率需要case 第二列, 或者可以两列都写
    */
    switch (v)
    {
    case 0x07: case 0x8f:
    case 0x70: case 0xf8:  return 0;
    case 0x0b: case 0xf4:  return 1;
    case 0xb0: case 0x4f:  return 4;
    case 0x0d: case 0xf2:  return 2;
    case 0xd0: case 0x2f:  return 8;
    case 0x0e: case 0xf1:  return 3;
    case 0xe0: case 0x1f:  return 12;
    default:
        return KEY_NULL;
    }
}

unsigned char KEY_4X4_Value(void)
{
    unsigned char n;
    PIN = 0x0f; // 第一次检测
    if (PIN != 0x0f)
    {
        n = PIN;
        KEY_4X4_DELAY; // 按键消抖
        if (n == PIN)
        {
            PIN = 0xf0; // 第二次检测
            return (SwitchValue(n) | SwitchValue(PIN));
        }
    }
    return KEY_NULL;
}
```

<div STYLE="page-break-after: always;"></div>


## BUZZER
例如 蜂鸣器引脚是 P14 在 '\_\_config\_\_.h' 中添加宏\
在 'main.c' 中添加 sbit 定义, 并根据设备操作\
为什么不直接在 'main.c' sbit, 为了统一在 '\_\_config\_\_.h' 中管理引脚 

```cpp
// __config__.h
#define DEFINE_BUZZER P1 ^ 4
```

```cpp
// main.c
sbit Buzzer = DEFINE_BUZZER; // 设置 Buzzer 的状态 改变蜂鸣器状态
// 对于无源蜂鸣器, 响 : buzzer = !buzzer; 对于有源 buzzer = 0 or 1; 试一下
```

<div STYLE="page-break-after: always;"></div>


## DC_MOTOR
例如 直流电机引脚是 P27 在 '\_\_config\_\_.h' 中添加宏\
在 'main.c' 中添加 sbit 定义, 并根据设备操作

```cpp
// __config__.h
#define DEFINE_DC_MOTOR P2 ^ 7
```

```cpp
// 使用示例
// main.c
sbit dcm = DEFINE_DC_MOTOR;
unsigned char dcm_count = 0;
unsigned char dcm_max = 250;
unsigned char dcm_pwm = 100;

char condition = 1; // 模拟条件, 实际上可以是比如按键

void int_t0(void) interrupt 1
{
    // PWM(脉宽调制)脉冲波
    if (++dcm_count < dcm_pwm) { dcm = 1; }
    else if (dcm_count < dcm_max) { dcm = 0; }
    else { dcm_count = 0; }
    // 模拟条件切换
    if (dcm_count = 0)
        condition = (condition + 1) & 3;
}

void main(void)
{
    EA = 1;
	ET0 = 1;
	TMOD = 0x01;
	TH0 = TL0 = 0x00;
	TR0 = 1;
    while (1)
    {
        if (condition == 0)
            dcm_pwm = 0; // 关闭
        else if (condition == 1)
            dcm_pwm = 100; // 1档
        else if (condition == 2)
            dcm_pwm = 180; // 2档
        else if (condition == 3)
            dcm_pwm = dcm_max; // 最大
    }
}

```

<div STYLE="page-break-after: always;"></div>


## STEP_MOTOR
在 'main.c' 或 'step_motor.c' 中添加 相关定义, 并根据设备操作\
假设: 不仅电机引脚为 P2的低4位, 两个外部中断分别控制正传反转

```cpp
// 使用示例
// main.c
unsigned char spm_i = 0;
unsigned char code spm_turn[] = { 0x02, 0x06, 0x04, 0x0c, 0x08, 0x09, 0x01, 0x03, };
#define STEP_MOTOR_NEXT  spm_turn[(spm_i = (spm_i + 1) & 7)]
#define STEP_MOTOR_LAST  spm_turn[(spm_i = (spm_i - 1) & 7)]

void main(void)
{
    EA = 1;
    EX0 = 1; // 按下外部中断0按键, 正转
    EX1 = 1; // 按下外部中断1按键, 反转
    while (1);
}

void int_x0(void) interrupt 0
{
    P2 = STEP_MOTOR_NEXT;
    Delay1ms(1);
}

void int_x1(void) interrupt 2
{
    P2 = STEP_MOTOR_LAST;
    Delay1ms(1);
}
```


# 第四步: 编译通过烧入程序
