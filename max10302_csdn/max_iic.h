#ifndef _MAX_IIC_H
#define _MAX_IIC_H

#include "stm32l1xx_hal.h"
#include "stdint.h"
#include "main.h"

#define SCL_H              HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)
#define SCL_L              HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)

#define SDA_H              HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET)
#define SDA_L              HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET)
#define MAX_INT_H          HAL_GPIO_WritePin(MAX_INT_GPIO_Port, MAX_INT_Pin, GPIO_PIN_SET)
#define SCL_read           HAL_GPIO_ReadPin(SCL_GPIO_Port,SCL_Pin)
#define SDA_read           HAL_GPIO_ReadPin(SDA_GPIO_Port,SDA_Pin)
#define MAX_INT_read       HAL_GPIO_ReadPin(MAX_INT_GPIO_Port,MAX_INT_Pin)
#define bool int
#define TRUE 1
#define FALSE 0

void I2C_delay(void);
bool I2C_Start(void);
void I2C_Stop(void) ;
void I2C_Ack(void);
void I2C_NoAck(void);
uint8_t I2C_WaitAck(void);
void I2C_SendByte(uint8_t SendByte);
uint8_t I2C_ReceiveByte(uint8_t ack);
bool I2C_Write(uint8_t dev,uint8_t WriteAddr,uint8_t WriteData);
bool I2C_Write2(uint8_t WriteAddr,uint8_t WriteData);
uint8_t I2C_Read(uint8_t dev,uint8_t WriteAddr);
uint8_t I2C_Read2(uint8_t WriteAddr) ;
void I2C_DevRead(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *rbuf);
void I2C_DevWrite(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *wbuf);
void IIC_WriteBytes(uint8_t WriteAddr,uint8_t* data,uint8_t dataLength);
void IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr,uint8_t* data,uint8_t dataLength);
void IIC_Read_One_Byte(uint8_t daddr,uint8_t addr,uint8_t* data);
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
#endif
