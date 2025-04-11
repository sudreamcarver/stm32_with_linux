#include "serial.h"
#include <Delay.h>
#include <Key.h>
#include <stdint.h>

uint8_t received_data;
extern uint8_t TxDatapkg[4];

int main(void) {

  serial_init();

  TxDatapkg[0] = 0x01;
  TxDatapkg[1] = 0x02;
  TxDatapkg[2] = 0x03;
  TxDatapkg[3] = 0x04;

  serial_sendpkg();

  // serial_sendbyte('G');
  // uint8_t DFG[] = {0x42, 0x41, 0x45, 0x22};

  // serial_sendarray(DFG, 4);

  while (1) {
  }
}
