#ifndef __CONFIG___H
#define __CONFIG___H

#include <REG52.H>

// lcd1602
#define LCD1602_DATA P0             // 数据口
#define LCD1602_DEFINE_RS P1 ^ 0    // 寄存器选择
#define LCD1602_DEFINE_RW P1 ^ 1    // 读/写
#define LCD1602_DEFINE_EN P1 ^ 2    // 使能

// ds18b20
extern void Delay10us(unsigned char t);
#define DS18B20_DELAY_10US_FN Delay10us // 定义延迟函数函数宏, 必须要可传一个参数
#define DS18B20_DEFINE_DQ P1 ^ 5        // ds18b20 单总线

// hc_sr04
extern void Delay10us(unsigned char t);
#define HC_SR04_DELAY_10US Delay10us(1) // 定义延迟函数调用宏 触发信号需保持 10us 以上
#define HC_SR04_DEFINE_TRIG P3 ^ 4      // 触发信号输 
#define HC_SR04_DEFINE_ECHO P3 ^ 5      // 回响信号输出

// key_4x4
extern void Delay1ms(unsigned int t);
#define KEY_4X4_DELAY Delay1ms(10)  // 定义延迟函数调用宏, 消抖
#define KEY_4X4_PIN P2

// buzzer
#define DEFINE_BUZZER P1 ^ 4

// mc motor
#define DEFINE_DC_MOTOR P1 ^ 0

#endif
