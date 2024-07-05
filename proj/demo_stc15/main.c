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

#include "system.h"

// 24年 周六 5月 18日 14时 9分 40秒
code u8 date_time[] = {40, 9, 14, 18, 5, 6, 24};

sbit led    = LED;
sbit relay  = RELAY;
sbit buzzer = BUZZER;
sbit dcm    = DCM;

#if MODULE == 0
sbit pwm_led = PWM_LED;
sbit inf     = INF;
#endif

void init(void)
{
    void io_init(void);
    void reg_init(void);
    void module_init(void);

    io_init();
    module_init();

    lcd_init();
    ds1302_init();
    pwm_init();
}

#include "mp3player.h"
#include "pff.h"
#include "sd.h"
#include "spi.h"
#include "vs1053.h"

FATFS fatfs;              // 文件系统结构体定义
u8 tbuf[512];             // 512字节 SD卡数据缓存空间

void main(void)
{
    // init();

    // lcd_action();

    // while (1)
    // {

    // }

#if 1
    // mp3
    FRESULT res;
    u16 br;
    u32 cnt = 0; // 音乐进度递增变量
    u8 mp3in;

    SP = 0X80; // 调整堆栈指向

    // init();
    io_init();
    lcd_init();
    spi_init();
    SD_Init(); // SD卡初始化

    pf_mount(&fatfs); // 初始化petit FATFS文件系统  并提取tf卡相应数据
        // 这句非常重要，它是使用所有Petit Fatfs文件系统功能的前提

    lcd_cmd(0x80);
    lcd_printf("VS1053--TEST");

    mp3in = Mp3Player_Init();

    lcd_cmd(0x90);
    if (mp3in == 1)
        lcd_printf("MP3 Init OK");
    else
        lcd_printf("MP3 Init Error");

    res = pf_open("/MUSIC/练习.mp3"); // 打开指定路径下的音乐文件名

    lcd_cmd(0x88);
    if (res == FR_OK)
        lcd_printf("Already open");

    VS_Restart_Play();     // 重启播放
    VS_Set_All();          // 设置音量等信息
    VS_Reset_DecodeTime(); // 复位解码时间
    VS_Set_Vol(220);       // 设置音量
    VS_SPI_SpeedHigh();    // SPI设置为高速

    lcd_cmd(0x98);
    lcd_printf("Play music...");

    led = 1;

    while (1)
    {
        res =
            pf_read(tbuf, 512, &br); //通过文件系统读取指定文件夹下的一音乐数据

        if ((res != FR_OK))
        {
            led = 0;
            while (1)
                ;
        }
        cnt = 0;

        do
        {
            if (VS_Send_MusicData(tbuf + cnt) == 0) //一次送32个字节音乐数据
            {
                cnt += 32;
            }
            else
            {
                led = 0;
            }
        } while (cnt < 512);
    }
    if (br != 512) // 文件结束
    {
        while (1)
            ;
    }
#endif

#if 0
    // pwm
    pwm_clk(0x0b); // clk: 2MHz

    pwm_set(0, 10, 1000); // led: 2000Hz

    PWMCR |= 0x80;  // 使能PWM波形发生器，PWM计数器开始计数
    PWMCR &= ~0x40; // 禁止PWM计数器归零中断

    while (1)
        ;
#endif

#if 0
    // 温湿度
    while (1)
    {
        delay_1ms(1000);
        // flg = dht11_read(&r, &t);
        lcd_cmd(0x01);
        // lcd_printf("%d R: %2.0f  T: %4.1f", (u16)flg, r, t);
    }
#endif

    ds18b20_convert();
    ds1302_init();
#if 0
    ds1302_set(date_time);
#endif
    pwm_init();

    P15 = P16 = 0;

    if (key_value() == 0x0f)
    {
        lcd_cmd(0x80);
        lcd_printf("About To");
        lcd_cmd(0x90);
        lcd_printf("Reset Password");
        while (key_value() != 0xff)
            ;
        reset_password();
    }

    enter_password();

    // while (1)
    // {
    //     updateShow();

    //     if (get_key() != 0xff)
    //     {
    //         switch (key)
    //         {
    //         case 0:
    //             TempMotorRun(0, 512);
    //             break;
    //         case 1:
    //             TempMotorRun(1, 512);
    //             break;
    //         case 2:
    //             P15 = !P15;
    //             P16 = !P16;
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    // }
}

#define PM_Z(P)    (P##M1 = P##M0 = 0x00)             // 置零
#define _PPO(P, B) (BIT_L(P##M1, B), BIT_H(P##M0, B)) // 推挽输出
#define PPO(P)     _PPO(P)

void io_init(void)
{
    PM_Z(P0);
    PM_Z(P1);
    PM_Z(P2);
    PM_Z(P3);
    PM_Z(P4);
    PM_Z(P5);
    PM_Z(P6);
    PM_Z(P7);

    // 公共资源推挽

    // LCD12864
    PPO(_LCD_RS); // P20
    PPO(_LCD_RW); // P21
    PPO(_LCD_EN); // P22

    // 继电器
    // PPO(_RELAY); // P52

    // 蜂鸣器
    // PPO(_BUZZER); // P55

    relay  = 0;
    led    = 1; // 1: 关闭
    buzzer = 1; // 1: 关闭
}

void module_init(void)
{
    // 直流电机
    PPO(_DC_MOTOR); // P15: 智慧农业; P16: 智能小车 可 pwm
    dcm = 0;
#if MODULE == 0
    // 调光LED
    PPO(_PWM_LED);
    pwm_led = 0;
#elif MODULE == 1
    // 步进电机
    PPO(_STEP_MOTOR + 0); // P11, 和智慧农业的温湿度冲突
    PPO(_STEP_MOTOR + 1); // P12
    PPO(_STEP_MOTOR + 2); // P13
    PPO(_STEP_MOTOR + 3); // P14, 和智慧农业的红外冲突
    // 红外测速
    IT1 = 1;
    EX1 = 1;
#endif
}

void reg_init(void)
{
#if 0 
    SCON = 0x50; //REN=1允许串行接受状态，串口工作模式2
    TMOD = 0x00; //定时器1为模式0（16位自动重载）
    AUXR = 0X40; //开启1T模式
    TL1 = (65535 - (24000000 / 4 / 9600)); //设置波特率重装值
    TH1 = (65535 - (24000000 / 4 / 9600)) >> 8;
    TR1 = 1; //开启定时器1
    ES = 1;  //开串口中断
#endif
    T4T3M &= 0xFD; // 定时器时钟12T模式
    T3L = 0xB0;    // 设置定时初始值
    T3H = 0x3C;    // 设置定时初始值 25ms
    T4T3M |= 0x08; // 定时器3开始计时

    EA = 1;
    IE2 |= 0x20; // 定时器3中断
    // IE2 |= 0x04; // 定时器2中断

    // IT1 = 1;
    // EX1 = 1;
}

void int_x1(void) interrupt 2
{
    count_x1++;
}

void int_t3(void) interrupt 19
{
    count_t3++;
}
