#ifndef __I2C_H
#define __I2C_H
#include <stdint.h>
void MY_I2C_W_SCL(uint8_t BitValue);
void MY_I2C_W_SDA(uint8_t BitValue);
uint8_t MY_I2C_R_SDA(void);
uint8_t MY_I2C_R_SCL(void);
void MY_I2C_GpioInit(void);
void MY_I2C_Start(void);
void MY_I2C_Stop(void);
void MY_I2C_SendByte(uint8_t Byte);
uint8_t MY_I2C_ReceiveByte(void);
void MY_I2C_SendACK(uint8_t AckBit);
uint8_t MY_I2C_ReceiveACK(void);
#endif
