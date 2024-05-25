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

#ifndef __CONFIG___H
#define __CONFIG___H

#include <STC15.H>

#include "__type__.h"

/* ========================================================================== */

#define MODULE 1 // 0: 智慧农业; 1: 智能小车; 2: 智能音箱; 3: 工业互联网

// 兼容 sbit 的引脚定义
#define _SBIT(P, B) P ^ (B)
#define SBIT(P)     _SBIT(P) // 延迟宏展开 : Px,B - > Px^B

/* ========================================================================== */
// 公共资源

// unused
#define UNUSED_DHT11
#define UNUSED_STEP_MOTOR
#define UNUSED_ADC
#define UNUSED_PWM
#define UNUSED_HC_SR04
#define UNUSED_DS18B20

// 计算 ADC
#define VOLTAGE 3.3
#define V_TOTAL 1024

// 需要定义推挽输出的引脚
#define _LCD_RS P2, 0
#define _LCD_RW P2, 1
#define _LCD_EN P2, 2
#define _RELAY  P5, 2
#define _BUZZER P5, 5
// _PWM_LED
// _DC_MOTOR
// _STEP_MOTOR

// main
#define LED    P5 ^ 3        // led
#define RELAY  SBIT(_RELAY)  // relay
#define BUZZER SBIT(_BUZZER) // buzzer
// DCM
// INF
// PWM_LED

// lcd12864
#define LCD_DATA P0
#define LCD_RS   SBIT(_LCD_RS)
#define LCD_RW   SBIT(_LCD_RW)
#define LCD_EN   SBIT(_LCD_EN)

// key_4x4
#define KEY_4X4_PIN   P7
#define KEY_4X4_DELAY delay_1ms(5)

// i2c, at24c
#define I2C_SDA P6 ^ 6
#define I2C_SCL P6 ^ 7

// ds1302
#define DS1302_SCK P3 ^ 5
#define DS1302_SDA P3 ^ 6
#define DS1302_CE  P5 ^ 4

/* ========================================================================== */
#if MODULE == 0 // 智慧农业

#undef UNUSED_DHT11
#undef UNUSED_ADC
#undef UNUSED_PWM

// dc motor
#define _DC_MOTOR P1, 5
#define DCM       SBIT(_DC_MOTOR)

// dht11
#define DHT11_DQ P1 ^ 1 // 和步进电机冲突, 设置为双向IO口

// adc
#define LDR 0 // 光敏电阻 P10

// 红外对管检测
#define INF P1 ^ 4

// pwm led
#define _PWM_LED P1, 7
#define PWM_LED  SBIT(_PWM_LED)
#define PWM_ID   7 // 7: P17

/* ========================================================================== */
#elif MODULE == 1 // 智能小车

#undef UNUSED_STEP_MOTOR
#undef UNUSED_HC_SR04
#undef UNUSED_DS18B20
#undef UNUSED_ADC
#undef UNUSED_PWM

// dc motor, 可以 pwm, 最好大于50%
#define _DC_MOTOR    P1, 6
#define DCM          SBIT(_DC_MOTOR)

// step motor
#define _STEP_MOTOR  P1, 1       // 风格保持一致
#define STEP_MOTOR   _STEP_MOTOR // 兼容库定义

// hc_sr04
#define HC_SR04_TRIG P1 ^ 5
#define HC_SR04_ECHO P3 ^ 4

// ds18b20
#define DS18B20_DQ   P1 ^ 7

// adc
#define POT          0 // 电位器 P10

// pwm dc motor
#define PWM_ID       6 // 6: P16

// 红外测速 P33 INT1(外部中断1)  IT1 = 1;  EX1 = 1;

/* ========================================================================== */
#elif MODULE == 2      // 智能音箱

#endif

/* ========================================================================== */

// 很多模块可能会用到延迟, 所以放在 config 中申明
extern void _nop_(void);
extern void delay_5us(u8 t);
extern void delay_1ms(u16 t);

// 定义一些有用的宏函数
#define BIT_H(N, B) (N) |= (1 << (B))
#define BIT_L(N, B) (N) &= ~(1 << (B))

static u16 s_count_i; // 在同一个源文件中不可以嵌套, 不同源文件中可以嵌套
#define for_c(N) for (s_count_i = (N); s_count_i; --s_count_i)

#endif // __CONFIG___H
