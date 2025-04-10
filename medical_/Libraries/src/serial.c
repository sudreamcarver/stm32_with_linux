#include "stm32f10x.h"
#include <stdint.h>

void serial_init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);
}

void serial_sendbyte(uint8_t Byte) {
  USART_SendData(USART1, Byte);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    ;
}

void serial_sendarray(uint8_t *Array, uint16_t Length) {
  uint16_t i;
  for (i = 0; i < Length; i++) {
    serial_sendbyte(Array[i]);
  }
}

void serial_sendstring(char *string) {
  uint8_t i;
  for (i = 0; string[i] != '\0'; i++) {
    serial_sendbyte(string[i]);
  }
}

uint32_t serial_pow(uint32_t X, uint32_t Y) {
  uint32_t Result = 1;
  while (Y--) {
    Result *= X;
  }
  return Result;
}

void serial_sendnumber(uint32_t Number, uint8_t Length) {
  uint8_t i;
  for (i = 0; i < Length; i++) {
    serial_sendbyte(Number / serial_pow(10, Length - i - 1) % 10 + '0');
  }
}
