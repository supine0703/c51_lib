#ifndef SPI_H
#define SPI_H

#include "__type__.h"

void spi_init(void); // SPI初始化

u8 spi_send_byte(u8 byte); // SPI为全双工通讯

void spi_speed(u8 speed); // SPI速度调节

#define SPI_SendByte spi_send_byte
#define SPI_Speed spi_speed

#endif // SPI_H
