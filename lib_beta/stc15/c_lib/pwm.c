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

#include "__config__.h"

#ifndef UNUSED_PWM

/* ========================================================================== */

#define EAXSFR BIT_H(P_SW2, 7)
#define EAXRAM BIT_L(P_SW2, 7)

#define PWM_X(X, _) PWM##X##_
#define PWM_CR(X)   PWM_X(X, CR)
#define PWM_T1(X)   PWM_X(X, T1)
#define PWM_T2(X)   PWM_X(X, T2)

void pwm_init(void)
{
    EAXSFR;             // 访问XFR
    PWM_CR(PWM_ID) = 0; // PWM7CR = 0;
    PWMCR |= (1 << (PWM_ID - 2)); // 相应的端口为PWM输出口，受PWM波形发生器控制
    PWMCFG &= ~(1 << (PWM_ID - 2)); // 0010 0000 PWM 初始低电平, 不自动触发ADC
    EAXRAM;
    // PWMCR &= ~0x40; // 禁止PWM计数器归零中断
    // PWMCR |= 0x80;  // 使能PWM波形发生器，PWM计数器开始计数
}

// 设置时钟源 0-15: sys_clk / (ps + 1); 16: t2
void pwm_clk(u8 ps)
{
    EAXSFR;             // 访问XFR
    PWMCKS = ps & 0x1f; // PWM时钟选择
    EAXRAM;             // 恢复访问XRAM
}

// 设置 pwm 波(周期和两次翻转)  1~32767(0x7fff)
void pwm_set(u16 first, u16 second, u16 cycles)
{
    EAXSFR;                  // 访问XFR
    PWM_T1(PWM_ID) = first;  // PWM7T1 = first;
    PWM_T2(PWM_ID) = second; // PWM7T2 = second;
    PWMC           = cycles;
    EAXRAM; // 恢复访问XRAM
}

void PWM_int(void) interrupt 22
{
    if (PWMIF & 0x40)
    {                   // PWM计数器归零中断标志
        PWMIF &= ~0x40; // 清除中断标志
    }
    if (PWMIF & 0x01)
    {                   // PWM2中断标志
        PWMIF &= ~0x01; // 清除中断标志
    }
    if (PWMIF & 0x02)
    {                   // PWM3中断标志
        PWMIF &= ~0x02; // 清除中断标志
    }
    if (PWMIF & 0x04)
    {                   // PWM4中断标志
        PWMIF &= ~0x04; // 清除中断标志
    }
    if (PWMIF & 0x08)
    {                   // PWM5中断标志
        PWMIF &= ~0x08; // 清除中断标志
    }
    if (PWMIF & 0x10)
    {                   // PWM6中断标志
        PWMIF &= ~0x10; // 清除中断标志
    }
    if (PWMIF & 0x20)
    {                   // PWM7中断标志
        PWMIF &= ~0x20; // 清除中断标志
    }
}

void PWMFD_int(void) interrupt 23
{
    if (PWMFDCR & 0x01)
    {                     // PWM异常检测中断标志位
        PWMFDCR &= ~0x01; // 清除中断标志
    }
}

/* ========================================================================== */

#endif // UNUSED_PWM
