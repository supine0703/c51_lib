/*
    This file is part of the c51_lib, see <https://github.com/supine0703/c51_lib>.
    
    Copyright (C) <2024>  <������>  <email: supine0703@outlook.com>

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

// 24�� ���� 5�� 18�� 14ʱ 9�� 40��
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

FATFS fatfs;              // �ļ�ϵͳ�ṹ�嶨��
u8 tbuf[512];             // 512�ֽ� SD�����ݻ���ռ�

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
    u32 cnt = 0; // ���ֽ��ȵ�������
    u8 mp3in;

    SP = 0X80; // ������ջָ��

    // init();
    io_init();
    lcd_init();
    spi_init();
    SD_Init(); // SD����ʼ��

    pf_mount(&fatfs); // ��ʼ��petit FATFS�ļ�ϵͳ  ����ȡtf����Ӧ����
        // ���ǳ���Ҫ������ʹ������Petit Fatfs�ļ�ϵͳ���ܵ�ǰ��

    lcd_cmd(0x80);
    lcd_printf("VS1053--TEST");

    mp3in = Mp3Player_Init();

    lcd_cmd(0x90);
    if (mp3in == 1)
        lcd_printf("MP3 Init OK");
    else
        lcd_printf("MP3 Init Error");

    res = pf_open("/MUSIC/��ϰ.mp3"); // ��ָ��·���µ������ļ���

    lcd_cmd(0x88);
    if (res == FR_OK)
        lcd_printf("Already open");

    VS_Restart_Play();     // ��������
    VS_Set_All();          // ������������Ϣ
    VS_Reset_DecodeTime(); // ��λ����ʱ��
    VS_Set_Vol(220);       // ��������
    VS_SPI_SpeedHigh();    // SPI����Ϊ����

    lcd_cmd(0x98);
    lcd_printf("Play music...");

    led = 1;

    while (1)
    {
        res =
            pf_read(tbuf, 512, &br); //ͨ���ļ�ϵͳ��ȡָ���ļ����µ�һ��������

        if ((res != FR_OK))
        {
            led = 0;
            while (1)
                ;
        }
        cnt = 0;

        do
        {
            if (VS_Send_MusicData(tbuf + cnt) == 0) //һ����32���ֽ���������
            {
                cnt += 32;
            }
            else
            {
                led = 0;
            }
        } while (cnt < 512);
    }
    if (br != 512) // �ļ�����
    {
        while (1)
            ;
    }
#endif

#if 0
    // pwm
    pwm_clk(0x0b); // clk: 2MHz

    pwm_set(0, 10, 1000); // led: 2000Hz

    PWMCR |= 0x80;  // ʹ��PWM���η�������PWM��������ʼ����
    PWMCR &= ~0x40; // ��ֹPWM�����������ж�

    while (1)
        ;
#endif

#if 0
    // ��ʪ��
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

#define PM_Z(P)    (P##M1 = P##M0 = 0x00)             // ����
#define _PPO(P, B) (BIT_L(P##M1, B), BIT_H(P##M0, B)) // �������
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

    // ������Դ����

    // LCD12864
    PPO(_LCD_RS); // P20
    PPO(_LCD_RW); // P21
    PPO(_LCD_EN); // P22

    // �̵���
    // PPO(_RELAY); // P52

    // ������
    // PPO(_BUZZER); // P55

    relay  = 0;
    led    = 1; // 1: �ر�
    buzzer = 1; // 1: �ر�
}

void module_init(void)
{
    // ֱ�����
    PPO(_DC_MOTOR); // P15: �ǻ�ũҵ; P16: ����С�� �� pwm
    dcm = 0;
#if MODULE == 0
    // ����LED
    PPO(_PWM_LED);
    pwm_led = 0;
#elif MODULE == 1
    // �������
    PPO(_STEP_MOTOR + 0); // P11, ���ǻ�ũҵ����ʪ�ȳ�ͻ
    PPO(_STEP_MOTOR + 1); // P12
    PPO(_STEP_MOTOR + 2); // P13
    PPO(_STEP_MOTOR + 3); // P14, ���ǻ�ũҵ�ĺ����ͻ
    // �������
    IT1 = 1;
    EX1 = 1;
#endif
}

void reg_init(void)
{
#if 0 
    SCON = 0x50; //REN=1�����н���״̬�����ڹ���ģʽ2
    TMOD = 0x00; //��ʱ��1Ϊģʽ0��16λ�Զ����أ�
    AUXR = 0X40; //����1Tģʽ
    TL1 = (65535 - (24000000 / 4 / 9600)); //���ò�������װֵ
    TH1 = (65535 - (24000000 / 4 / 9600)) >> 8;
    TR1 = 1; //������ʱ��1
    ES = 1;  //�������ж�
#endif
    T4T3M &= 0xFD; // ��ʱ��ʱ��12Tģʽ
    T3L = 0xB0;    // ���ö�ʱ��ʼֵ
    T3H = 0x3C;    // ���ö�ʱ��ʼֵ 25ms
    T4T3M |= 0x08; // ��ʱ��3��ʼ��ʱ

    EA = 1;
    IE2 |= 0x20; // ��ʱ��3�ж�
    // IE2 |= 0x04; // ��ʱ��2�ж�

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
