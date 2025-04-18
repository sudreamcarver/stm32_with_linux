#include "Delay.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>

void MY_I2C_W_SCL(uint8_t BitValue) {
  GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
  Delay_us(10);
}
void MY_I2C_W_SDA(uint8_t BitValue) {
  GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
  Delay_us(10);
}

uint8_t MY_I2C_R_SDA(void) {
  uint8_t BitValue;
  BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
  return BitValue;
}
uint8_t MY_I2C_R_SCL(void) {
  uint8_t BitValue;
  BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
  return BitValue;
}
void MY_I2C_GpioInit(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void MY_I2C_Start(void) {
  MY_I2C_W_SCL(1);
  MY_I2C_W_SDA(1);
  MY_I2C_W_SDA(0);
  MY_I2C_W_SCL(1);
}

void MY_I2C_Stop(void) {
  MY_I2C_W_SDA(0);
  MY_I2C_W_SCL(1);
  MY_I2C_W_SDA(1);
}

void MY_I2C_SendByte(uint8_t Byte) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    MY_I2C_W_SDA(Byte & 0x80 >> i);
    MY_I2C_W_SCL(1);
    MY_I2C_W_SCL(0);
  }
}

uint8_t MY_I2C_ReceiveByte(void) {
  uint8_t i, Byte = 0x00;
  MY_I2C_W_SDA(1);
  for (i = 0; i < 8; i++) {
    MY_I2C_W_SCL(1);

    if (MY_I2C_R_SDA() == 1) {
      Byte |= (0x80 >> i);
    }

    MY_I2C_W_SCL(0);
  }
  return Byte;
}

void MY_I2C_SendACK(uint8_t AckBit) {
  MY_I2C_W_SDA(AckBit);
  MY_I2C_W_SCL(1);
  MY_I2C_W_SCL(0);
}

uint8_t MY_I2C_ReceiveACK(void) {
  uint8_t AckBit;
  MY_I2C_W_SDA(1);
  MY_I2C_W_SCL(1);
  AckBit = MY_I2C_R_SDA();
  MY_I2C_W_SCL(0);
  return AckBit;
}
