#include "ledController.h"

#include <avr/io.h>         // Include header file for AVR I/O register definitions
#include <util/delay.h>     // Include header file for delay functions
#include <stdio.h>          // Include header file for standard I/O operations
#include <avr/interrupt.h>  // Include header file for interrupt handling

#define F_CPU 16000000UL    // Define the CPU frequency, required for delay functions (16 MHz)

// ===================================================================
// Public Methods
// ===================================================================
void LedController::setGreetingMsg(const char* msg)
{
  _greetingMsg = msg;
}

// Using int so we dont have to import bool
unsigned short int firstTimeFlag = 1;

void LedController::run(void)
{

    if (firstTimeFlag == 1)
    {
      _USART_transmit_string(_greetingMsg);

      if (firstTimeFlag == 1)
      {
        firstTimeFlag = 0;
      }
    }

    // Wait for a character from the serial input
    unsigned char receivedChar = _USART_receive_char();

    // Check if the received character is the onChar or offChar
    if (receivedChar == _onChar)
    {
        // Turn the LED on
        PORTB |= (1 << PORTB5);
        // Transmit the string "LED turned on!" using USART
        _USART_transmit_string("LED turned on!\r\n");
    }

    else if (receivedChar == _offChar)
    {
        // Turn the LED off
        PORTB &= ~(1 << PORTB5);
        // Transmit the string "LED turned off!" using USART
        _USART_transmit_string("LED turned off!\r\n");
    }

    else
    {
        _USART_transmit_string("Char not recognized!\r\n");
    }
}

// ===================================================================
// Private Methods
// ===================================================================

// Function to initialize the USART (Universal Synchronous Asynchronous Receiver Transmitter) communication
void LedController::_USART_init(unsigned int baud)
{
  unsigned int ubrr = _calcBaud(baud);
  UBRR0H = (unsigned char)(ubrr >> 8);   // Set the high byte of the USART baud rate register
  UBRR0L = (unsigned char)ubrr;          // Set the low byte of the USART baud rate register

  UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable USART receiver and transmitter
  UCSR0C = (1 << USBS0) | (3 << UCSZ00); // Set USART frame format: 8 data bits, 2 stop bits
}

unsigned int LedController::_calcBaud(unsigned int baud)
{
  return (F_CPU / ((16 * baud) - 1));
}

// Function to transmit a character using USART
void LedController::_USART_transmit_char(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));      // Wait for the transmit buffer to be empty
    UDR0 = data;                           // Put the character in the USART data register
}

// Function to transmit a string using USART
void LedController::_USART_transmit_string(const char *str)
{
    while (*str)
    {
     _USART_transmit_char(*str++);       // Transmit each character in the string
    }
}


// Function to receive a character using USART
unsigned char LedController::_USART_receive_char(void)
{
    while (!(UCSR0A & (1 << RXC0))); // Wait for the receive buffer to have data
    return UDR0;                    // Return the received character
}


