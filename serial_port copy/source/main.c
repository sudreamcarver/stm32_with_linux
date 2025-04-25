#include "OLED.h"
#include "serial.h"
#include <Delay.h>
#include <Key.h>
#include <stdint.h>

uint8_t received_data;

int main(void) {

  OLED_Init();
  serial_init();

  serial_sendbyte('G');
  uint8_t DFG[] = {0x42, 0x41, 0x45, 0x22};

  serial_sendarray(DFG, 4);

  OLED_ShowString(1, 1, "RxData:");

  while (1) {
    if (serial_GetRxFlag() == 1) {
      received_data = serial_GetRxData();
      serial_sendstring("end\r\n");
    }
    serial_sendnumber(received_data, 2);
    OLED_ShowNum(2, 1, received_data, 3);
    Delay_ms(500);
  }
}
