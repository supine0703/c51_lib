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

#ifndef DC_MOTOR_H
#define DC_MOTOR_H

/**
 * 使用建议:
 *   - 在 '__config__.h' 中定义宏 DEFINE_DC_MOTOR
 *     然后再需要使用的 .c 文件中 定义 sbit dcm = DEFINE_DC_MOTOR
 *  
 * 直流电机最常见的驱动方式是: 施加PWM(脉宽调制)脉冲波
 * 即 在一个时间周期内, 调整运行的时间占比来控制直流电机的转速
 * 
 * 运行占比的对 DC-motor 影响:
 *   - 在最大速度内: 占比越大, 速度越快
 *   - 达到最大速度: 占比越大, 高速运转持续时间越长
 *   - 在非零速度时: 占比越小, 速度越慢
 *   - 在速度停止时: 占比越小, 停息时间越长
 * 
 * 周期对 DC-motor 影响:
 *   - 最重要的: 直流电机驱动需要克服阻力, 如果周期越短, 占比可靠性越差, 
 *             甚至可能出现驱动时间过短, 导致电机无法驱动成功
 *             但若周期太长, 更明显的为间歇性运行的变化, 而不是转速变化
 *   - 其他的影响道理同上, 就没有必要赘述了, 总之为正比关系
 * 
 * 使用方法为:
 * 1. 定义: 周期计数最大值: max, 脉宽调制(运行占比): pwm(run), 计数器: count
 * 2. 条件判断: 
        if (++count < pwm) { dcm = 1; }
        else if (count < max) { dcm = 0; }
        else { count = 0; }
 * 或者 (下面思路是临态切换, 建议使用上面 虽然代码多一行):
        if (++count == max) { dcm = 1; count = 0; }
        else if (count == pwm) { dcm = 0; }
 * 注意: 不要调换两个判断条件的顺序, dcm = 1 优先级比 dcm = 0 高
*/

#endif // DC_MOTOR_H
