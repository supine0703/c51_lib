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