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
