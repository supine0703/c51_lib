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

#ifndef HC_SR04_H
#define HC_SR04_H
/**
 * 在使用之前需要在 '__config__.h' 中定义以下宏:
 *   - HC_SR04_DELAY_10US: 对保持触发信号的延迟调用定义 必须大于 10us 否则...
 *   - HC_SR04_DEFINE_TRIG: 定义触发信号的使用引脚
 *   - HC_SR04_DEFINE_ECHO: 定义回响信号的使用引脚
 *   - HC_SR04_TIMER: 0/1 定义 HC_SR04 使用的定时器
 *                    目前仅支持: T0, T1 (T2用在这不太必要)
 * 
 * 当包含多文件测试时, 有些文件暂时用不了 Keil 会警告, 作妥协可以定义宏: 
 *   - UNUSED_HC_SR04: 这将不会编译 HC_SR04 相关函数
*/

/* ========================================================================== */

/**
 * 注意事项:
 *   - 探测并返回, 整个过程不会用到中断, 请勿开启所使用的定时器中断('__config__.h'中定义的)
 *     虽然一般情况不会出错, 但是难免会有意外
 *   - 虽然不会用到中断, 但是如果有中断打断过程可能会造成以下问题：
 *       - 发送触发信号被打断, 导致探测失败, 将会返回不确定的数字
 *       - 计时被干扰, 导致计时错误, 会返回不确定的数字
 *   - 此封装, 虽然对 TMOD 进行了操作, 但是不会干扰到另一个定时器工作
 *   - 为了防止 TMOD 在外部赋值出现意外, 再每次开始前都会对定时器初始化, 略微的开销是值得的
 * 返回值: 计算出的距离, 单位: mm
*/
unsigned int HC_SR04_Millimeter(void);

#endif // HC_SR04_H
