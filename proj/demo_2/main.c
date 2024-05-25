#include "__config__.h"

unsigned char spm_i = 0;
unsigned char code spm_turn[] = {
    0x02,
    0x06,
    0x04,
    0x0c,
    0x08,
    0x09,
    0x01,
    0x03,
};
#define STEP_MOTOR_NEXT spm_turn[(spm_i = (spm_i + 1) & 7)]
#define STEP_MOTOR_LAST spm_turn[(spm_i = (spm_i - 1) & 7)]

void Delay1ms(unsigned int t)
{
    unsigned char i;
    while (t--)
    {
        i = 123;
        while (i--)
            ;
    }
}

void init(void)
{
    EA = 1;
    EX0 = 1;
    EX1 = 1;
}

void main(void)
{

    init();

    while (1)
    {
    }
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
