#include "__config__.h"
#include "key_4x4.h"

#ifndef KEY_4X4_DELAY
#define KEY_4X4_DELAY
#endif // KEY_4X4_DELAY

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