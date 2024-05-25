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
    return ((v >> 2) - ((v / 25) << 1));
}