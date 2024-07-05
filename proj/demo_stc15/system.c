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

#define KEYS_MAX  8
#define KEYS_ADDR 0x0000

extern u8 buff[];

void lcd_init(void)
{
    lcd_cmd(0x01);
    lcd_cmd(0x0e);
    lcd_cmd(0x30);
}

void lcd_action(void)
{
    u8 code s1[17] = "      ABCD      ";
    u8 code s2[17] = "       07       ";
    u8 i;

    // 字从下往上, 屏幕从上往下 0x41 -> 0x50 -> 0x60
    lcd_cmd(0x01);

    lcd_cmd(0x88); // 第三行 -> 屏幕外 -> 屏幕外
    lcd_printf(s1);
    lcd_cmd(0xa0); // 屏幕外 -> 第二行 -> 第一行
    lcd_printf(s1);

    lcd_cmd(0x98); // 第四行 -> 第三行 -> 屏幕外
    lcd_printf(s2);
    lcd_cmd(0xb0); // 屏幕外 -> 屏幕外 -> 第二行
    lcd_printf(s2);

    lcd_cmd(0x34);
    lcd_cmd(0x03);

    i = 0x41;
    for_c(16)
    {
        delay_1ms(1000 / 16);
        lcd_cmd(i++);
    }

    delay_1ms(1000);

    for_c(16)
    {
        delay_1ms(1000 / 16);
        lcd_cmd(i++);
    }

    lcd_cmd(0x30);
    lcd_cmd(0x01);
}

u8 get_key(void)
{
    key = key_value();
    if ((key = key_value()) != 0xff)
    {
        while (key_value() != 0xff)
            ; // key 和 value 都不等于 0xff, 即松开后(0xff)返回按键值
    }
    return key;
}

void reset_password(void)
{
    bit loop = 1;
    u8 i     = 0, num;

    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_printf("New Password:");
    lcd_cmd(0x90);
    lcd_cmd(0x0e); // 开启光标

    while (loop)
    {
        num = 0xff;
        if (get_key() != 0xff)
        {
            switch (key)
            {
            case 0x00:
                num = 1;
                break;
            case 0x01:
                num = 2;
                break;
            case 0x02:
                num = 3;
                break;
            case 0x04:
                num = 4;
                break;
            case 0x05:
                num = 5;
                break;
            case 0x06:
                num = 6;
                break;
            case 0x08:
                num = 7;
                break;
            case 0x09:
                num = 8;
                break;
            case 0x0a:
                num = 9;
                break;
            case 0x0d:
                num = 0;
                break;
            case 0x03: // 退格
                if (i != 0)
                {
                    i--;
                    lcd_cmd(0x10);
                    lcd_printf("  ");
                    lcd_cmd(0x10);
                }
                break;
            case 0x0f: // 确认
                loop = 0;
                at24c_write(KEYS_ADDR, &i, 1);
                at24c_write(KEYS_ADDR + 1, buff, i);
                break;
            default:
                break;
            }
            if (num != 0xff && i < KEYS_MAX)
            {
                buff[i] = num + '0';
                lcd_show(buff[i]);
                lcd_show(0x20);
                num = 0xff;
                ++i;
            }
        }
    }
    lcd_cmd(0x0c); // 关闭光标
    lcd_cmd(0x01);
}

void enter_password(void)
{
    bit loop, lock = 1;
    u8 right, num, len, i;

    at24c_read(KEYS_ADDR, &len, 1);
    at24c_read(KEYS_ADDR + 1, buff + KEYS_MAX, len);

    while (lock)
    {
        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_printf("Enter Password:");
        lcd_cmd(0x90);
        lcd_cmd(0x0e);

        loop  = 1;
        right = 0;
        i     = 0;

        while (loop)
        {
            num = 0xff;
            if (get_key() != 0xff)
            {
                switch (key)
                {
                case 0x00:
                    num = 1;
                    break;
                case 0x01:
                    num = 2;
                    break;
                case 0x02:
                    num = 3;
                    break;
                case 0x04:
                    num = 4;
                    break;
                case 0x05:
                    num = 5;
                    break;
                case 0x06:
                    num = 6;
                    break;
                case 0x08:
                    num = 7;
                    break;
                case 0x09:
                    num = 8;
                    break;
                case 0x0a:
                    num = 9;
                    break;
                case 0x0d:
                    num = 0;
                    break;
                case 0x03: // 退格
                    if (i != 0)
                    {
                        lcd_cmd(0x10);
                        lcd_printf("  ");
                        if (right == i)
                        {
                            right--;
                        }
                        i--;
                        lcd_cmd(0x10);
                    }
                    break;
                case 0x0f: // 确认
                    loop = 0;
                    break;
                default:
                    break;
                }
                if (num != 0xff && i < KEYS_MAX)
                {
                    buff[i] = num + '0';
                    lcd_show(buff[i]);
                    lcd_show(0x20);
                    if (right == i)
                    {
                        right +=
                            ((buff[i] == buff[i + KEYS_MAX]) || (i >= len));
                    }
                    ++i;
                }
            }
        }
        lcd_cmd(0x0c);
        lcd_cmd(0x01);

        lock = (right != len); // 相等则密码正确

        lcd_cmd(0x90);
        if (!lock)
        {
            lcd_printf("Right Password!");
        }
        else
        {
            lcd_printf("Wrong Password!");
        }
        lcd_cmd(0x88);
        lcd_printf("Enter Any Key...");
        while (get_key() == 0xff)
            ;
    }
    lcd_cmd(0x01);
}

void updateShow(void)
{
    if (count_t3 > 20)
    {
        lcd_cmd(0x01);
        showTemperature();
        count_t3 = 0;

        lcd_cmd(0x9f);
        lcd_printf("%02d", (u16)count_x1);
        count_x1 = 0;

        lcd_cmd(0x86);
        lcd_printf("%4.2f", adc_result(LDR) * VOLTAGE / 1024); // 显示电压值
    }
    showTime();
}

void showTime(void)
{
    // lcd_cmd(0x80);
    // lcd_printf(
    //     "20%02d-%02d-%02d",
    //     (u16)DS1302_ReadBCD(0x8d),
    //     (u16)DS1302_ReadBCD(0x89),
    //     (u16)DS1302_ReadBCD(0x87)
    // );
    // lcd_cmd(0x90);
    // lcd_printf(
    //     "%02dh %02dm %02ds",
    //     (u16)DS1302_ReadBCD(0x85),
    //     (u16)DS1302_ReadBCD(0x83),
    //     (u16)DS1302_ReadBCD(0x81)
    // );
#if 1
    ds1302_get(buff);
    lcd_cmd(0x80);
    lcd_printf("20%02d-%02d-%02d", (u16)buff[6], (u16)buff[4], (u16)buff[3]);
    lcd_cmd(0x90);
    lcd_printf("%02dh %02dm %02ds", (u16)buff[2], (u16)buff[1], (u16)buff[0]);
#endif
}

void showTemperature(void)
{
    EA = 0;
    lcd_cmd(0x98);
    lcd_printf("T: %6.2f C", ds18b20_read_temp());
    ds18b20_convert();

    lcd_cmd(0x88);
    lcd_printf("D: %5.1f cm", hc_sr04_result());
    EA = 1;
}
