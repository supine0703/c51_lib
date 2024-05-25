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

#ifndef SYSTEM_H
#define SYSTEM_H

#include "__config__.h"
#include "__function__.h"

// variable
extern u8 key;
extern u8 count_x1;
extern u8 count_t3;

// lcd
void lcd_init(void); // lcd头文件提供最基础的操作, 初始化放在这合适一点
void lcd_action(void);

// key
u8 get_key(void);

// password
void reset_password(void);
void enter_password(void);

void updateShow(void);
void showTime(void);
void showTemperature(void);


#endif // SYSTEM_H
