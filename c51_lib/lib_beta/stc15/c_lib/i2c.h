#ifndef I2C_H
#define I2C_H

#include "__type__.h"

void I2C_Start(void);

void I2C_Stop(void);

void I2C_Ack(bit a);

bit I2C_CheckAck(void);

void I2C_Transmit(u8 dat);

u8 I2C_Receive(void);

#endif // I2C_H
