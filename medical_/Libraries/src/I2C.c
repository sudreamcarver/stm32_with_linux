#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>

#define max30102_control_addr 0xAE

void max30102_waitevent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT);

void i2c_init(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitTypeDef GPIO_initstructure;
  GPIO_initstructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_initstructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_initstructure.GPIO_Speed = GPIO_Speed_50MHz;

  I2C_InitTypeDef I2C_initstructure;
  I2C_initstructure.I2C_Ack = I2C_Ack_Enable;
  I2C_initstructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_initstructure.I2C_ClockSpeed = 50000;
  I2C_initstructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_initstructure.I2C_Mode = I2C_Mode_I2C;
  I2C_initstructure.I2C_OwnAddress1 = 0x00;
  I2C_Init(I2C2, &I2C_initstructure);

  I2C_Cmd(I2C2, ENABLE);
}

void I2C_writeREG(uint8_t REGaddr, uint8_t Data) {
  I2C_GenerateSTART(I2C2, ENABLE);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

  I2C_Send7bitAddress(I2C2, max30102_control_addr, I2C_Direction_Transmitter);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);

  I2C_SendData(I2C2, REGaddr);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);

  I2C_SendData(I2C2, Data);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t I2C_readREG(uint8_t REGaddr) {

  uint8_t Data;

  I2C_GenerateSTART(I2C2, ENABLE);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);

  I2C_Send7bitAddress(I2C2, max30102_control_addr, I2C_Direction_Receiver);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);

  I2C_SendData(I2C2, REGaddr);
  max30102_waitevent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);

  I2C_AcknowledgeConfig(I2C2, DISABLE);
  I2C_GenerateSTOP(I2C2, ENABLE);

  max30102_waitevent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
  Data = I2C_ReceiveData(I2C2);

  I2C_AcknowledgeConfig(I2C2, ENABLE);

  return Data;
}

void max30102_waitevent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT) {
  uint32_t Timeout;
  Timeout = 10000;
  while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS) {
    Timeout--;
    if (Timeout == 0) {
      break;
    }
  }
}
