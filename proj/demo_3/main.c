#include "__config__.h"
#include "delay.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "key_4x4.h"

sbit dcm = DEFINE_DC_MOTOR;
unsigned char dcm_count = 0;
unsigned char dcm_max = 250;
unsigned char dcm_pwm = 100;

sbit buzzer = DEFINE_BUZZER;
unsigned int buzzer_count = 0;

// c语言的标准格式化函数, 可以将数字等转为串再输出
extern int sprintf(char*, const char*, ...);

void LCD1602_Init(void);
void LCD1602_ShowString(unsigned char* s);

unsigned int convertCount = 0;
bit convert_finished = 0;

void init(void)
{
	EA = 1;
	ET0 = 1;
	TMOD = 0x02;
	TH0 = TL0 = 0x00;
	TR0 = 1;
}

void main(void)
{
	unsigned char str[20];
	unsigned char CC[3] = {0xdf, 0x43, 0};
	unsigned char key = 0;
	init();
	
	LCD1602_Init();
	
	while (1)
	{
		
//		DS18B20_Convert();
//		TR0 = 1;
//		while (!convert_finished);
//		sprintf(str, "%6.2f", DS18B20_ReadTemp() * 0.0625);
//		convert_finished = 0;
//		LCD1602_Cmd(Move_Cursor_Row1_Col(3));
//		LCD1602_ShowString(str);
//		LCD1602_ShowString(CC);
//		sprintf(str, "%dmm", HC_SR04_Millimeter());
//		LCD1602_ShowString(str);
//		Delay1ms(2000);
		key = KEY_4X4_Value();
		if (key != KEY_NULL)
		{
			while (KEY_4X4_Value() != KEY_NULL);
			key %= 6;
			dcm_pwm = key * 50;
			sprintf(str, "d:%d %d/%d", (int)key, (int)dcm_pwm, (int)dcm_max);
			LCD1602_Cmd(Clear_Screen);
			LCD1602_Cmd(Move_Cursor_Row1_Col(0));
			LCD1602_ShowString(str);
		}
	}
}

void LCD1602_Init(void)
{
	LCD1602_Cmd(Clear_Screen);
	LCD1602_Cmd(Mode_CursorRightMove);
	LCD1602_Cmd(Show_CursorOn);
	LCD1602_Cmd(Set_8bit_2line_5x7);
}

void LCD1602_ShowString(unsigned char* s)
{
    while (*s)
    {
        LCD1602_Write(*s++);
    }
}

void int_t0(void) interrupt 1
{
	// PWM(脉宽调制)脉冲波
    if (++dcm_count < dcm_pwm) { dcm = 1; }
    else if (dcm_count < dcm_max) { dcm = 0; }
    else { dcm_count = 0; }
//	if (++buzzer_count < 1000) buzzer = !buzzer;
//	else if (buzzer_count > 2000) buzzer_count = 0;
//    if (!convert_finished && ++convertCount >= 2700)
//    {
//        convertCount = 0;
//        convert_finished = 1;
//        TR0 = 0;
//        TH0 = TL0 = 0x00;
//    }
}
