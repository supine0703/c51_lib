#ifndef DS18B20_H
#define DS18B20_H

#define DS18B20_ERROR_T 0xffff

void DS18B20_Convert(); // 温度转换


int DS18B20_ReadTemp(); // 温度读取

#endif // DS18B20_H