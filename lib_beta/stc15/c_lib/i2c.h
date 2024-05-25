#ifndef I2C_H
#define I2C_H

#include "__type__.h"

void i2c_start(void);

void i2c_stop(void);

void i2c_ack(bit a);

bit i2c_check_ack(void);

void i2c_transmit(u8 dat);

u8 i2c_receive(void);

#endif // I2C_H
