#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <Delay.h>
#include <Key.h>
#include <stdint.h>

uint8_t KeyNum;

int main(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  Key_Init();

  while (1) {
    KeyNum = Key_GetNum();
    if (KeyNum == 1) {
      GPIO_WriteBit(GPIOA, GPIO_Pin_0,
                    (BitAction)!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0));
    }
    if (KeyNum == 2) {
      GPIO_WriteBit(GPIOA, GPIO_Pin_0,
                    (BitAction)!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0));
    }
  }
}
