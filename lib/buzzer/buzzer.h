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

#ifndef BUZZER_H
#define BUZZER_H

/**
 * 蜂鸣器的使用非常简单,
 *   - 单片机使用大多为有源蜂鸣器, 即 高低电平风别控制是否发声(具体看电路连接)
 *   - 还有无源蜂鸣器, 即 自己设置不同频率来控制音调, 需要脉冲信号控制
 *   - 无源: buzzer = !buzzer; // 响
 *   - 有源: buzzer = 0 or 1; // 根据电路连接, 试一下
 * 
 * 无源单片机声音单一, 但操作简单; 有源的操作复杂, 但可玩性更高, 
 * 复杂的蜂鸣器操作, 可参考我下面的开源项目中 <proj/proj_keil5_music>
 * <https://github.com/supine0703/c51_temperature_monitoring>
 *   - 这是部分是关于使用蜂鸣器奏乐的
 *   - 有源蜂鸣器也能有效果, 但是音色难评
 *   - 建议使用无源蜂鸣器进行操作
 * 
 * 使用建议:
 *   - 在 '__config__.h' 中定义宏 DEFINE_BUZZER
 *     然后再需要使用的 .c 文件中 定义 sbit buzzer = DEFINE_BUZZER
*/

#endif // BUZZER_H
