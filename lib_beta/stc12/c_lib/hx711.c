#include "__config__.h"

sbit DT = HX711_DT;
sbit SCK = HX711_SCK;

float HX711_Read() // 增益128
{
    u16 w;
    u8 i;

    DT = 1;
    SCK = 0;

    w = 0;

    while (DT)
        ;

    for (i = 16; i; --i)
    {
        SCK = 1;
        w <<= 1;
        SCK = 0;
        if (DT)
            w++;
    }

    return 0.256 * (w);
}
