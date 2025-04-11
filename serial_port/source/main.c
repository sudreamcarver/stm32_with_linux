#include "serial.h"
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <Delay.h>
#include <Key.h>
#include <stdint.h>

uint8_t received_data;

int main(void) {

  serial_init();

  // serial_sendbyte('G');
  // uint8_t DFG[] = {0x42, 0x41, 0x45, 0x22};

  // serial_sendarray(DFG, 4);

  while (1) {
    if (serial_GetRxFlag() == 1) {
      received_data = serial_GetRxData();
    }
    serial_sendnumber(received_data, 2);
    Delay_ms(500);
  }
}
