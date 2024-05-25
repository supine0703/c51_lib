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

#ifndef __FUNCTION___H
#define __FUNCTION___H

#include "__type__.h"

/* ========================================================================== */

// cstdio
extern int sprintf(char*, const char*, ...);
extern int vsprintf(char*, const char*, char*);

// delay
// extern void delay_5us(u8 t);
// extern void delay_1ms(u16 t);

// lcd12864
extern void lcd_cmd(u8 cmd);
extern void lcd_show(u8 dat);
extern void lcd_printf(const char* format, ...);

// key_4x4
extern u8 key_value(void);

// ds1302
extern void ds1302_init(void);
extern void ds1302_set(u8* set);
extern void ds1302_get(u8* get);

// at24c256
extern void at24c_read(u16 addr, u8* dat, u8 len);
extern void at24c_write(u16 addr, u8* dat, u8 len);

/* ========================================================================== */

// dht11
extern bit dht11_read(float* r, float* t);

// ds18b20
extern void ds18b20_convert();
extern float ds18b20_read_temp();

// hc_sr04
extern float hc_sr04_result(void);

// adc
extern u16 adc_result(u8 ch);

// step motor
extern void step_motor_run(bit s, u16 size);

// pwm
extern void pwm_init(void);
extern void pwm_clk(u8 ps);
extern void pwm_set(u16 first, u16 second, u16 cycles);

#endif // __FUNCTION___H
