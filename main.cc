#include "ledController.h"

int main(void)
{

  LedController nanoController(9600, '1', '0');
  nanoController.setGreetingMsg("Welcome! This is an example program for controlling an Arduino Nano via UART comm.");

  while(1)
  {
    nanoController.run();
  }

  return 0;
}
