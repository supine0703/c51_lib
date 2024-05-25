#include "__config__.h"
#include "delay.h"

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