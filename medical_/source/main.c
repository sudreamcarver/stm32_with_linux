#include "serial.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <Delay.h>
#include <Key.h>
#include <stdint.h>

uint8_t RxData = 1;

int main(void) {

  serial_init();

  // serial_sendbyte('G');
  // uint8_t DFG[] = {0x42, 0x41, 0x45, 0x22};

  // serial_sendarray(DFG, 4);

  while (1) {
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
      RxData = USART_ReceiveData(USART1);
    }
    serial_sendbyte(RxData);
    Delay_ms(500);
  }
}
