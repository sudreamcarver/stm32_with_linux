#include "serial.h"
#include <Delay.h>
#include <Key.h>
#include <stdint.h>
#include <stdio.h>

uint8_t KeyNum;

int main(void) {

  serial_init();

  // serial_sendbyte('G');
  // uint8_t DFG[] = {0x42, 0x41, 0x45, 0x22};

  // serial_sendarray(DFG, 4);

  while (1) {
    serial_sendstring("HelloWorld !\r\n");
    Delay_ms(500);
  }
}
