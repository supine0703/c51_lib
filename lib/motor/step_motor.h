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

#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H
/**
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作为妥协可以定义宏: 
 *   - UNUSED_STEPPING_MOTOR: 这将不会编译 STEPPING_MOTOR 相关函数
 * 
 * 使用建议:
 *   - 在 '__config__.h' 中定义宏 
*/

/* ========================================================================== */

/**
 * 应用方面, 步进电机比直流电机更加简单
 * 即 使用步进脉冲 + 延迟 进行循环即可
 *   - 正循环: 正转
 *   - 负循环: 负转
 *   - 顺时针还是逆时针取决于电路连接
 * 
 * 为了方便使用, 我定义了常用的步进脉冲数组 在 'stepping_motor.c' 中, 请通过宏调用:
 *   - STEP_MOTOR_NEXT: 获取下一个
 *   - STEP_MOTOR_LAST: 获取上一个
 * 
 * 为什么变量名/宏名这么长:
 *     因为 c语言 没有命名空间, 和私有封装, 只能通过加前后缀的方式, 避免未来可能出现的命名冲突
*/

extern unsigned char StepMotor_Index; // 步进电机的脉冲数组索引, 别乱动
extern unsigned char code StepMotor_Turns[]; // 步进电机的脉冲数组

#define STEP_MOTOR_NEXT                                                        \
    StepMotor_Turns[(StepMotor_Index = (StepMotor_Index + 1) & 7)]

#define STEP_MOTOR_LAST                                                        \
    StepMotor_Turns[(StepMotor_Index = (StepMotor_Index - 1) & 7)]

/* ========================================================================== */

#endif // STEP_MOTOR_H
