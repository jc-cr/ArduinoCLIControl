#include "ledController.h"

#include <avr/io.h>         // Include header file for AVR I/O register definitions
#include <util/delay.h>     // Include header file for delay functions
#include <stdio.h>          // Include header file for standard I/O operations
#include <avr/interrupt.h>  // Include header file for interrupt handling

// ===================================================================
// Public Methods
// ===================================================================


void LedController::setGreetingMsg(const char* msg)
{
  // Calculate the length of the input message
  char buf[150];
  sprintf(buf, "%s\r\n", msg);

  _greetingMsg = buf;
}

// Using int so we dont have to import bool
uint8_t connectionResetFlag = 1;

void LedController::run(void)
{

    if (connectionResetFlag == 1)
    {
      _USART_init(_baud);
      _USART_transmit_string(_greetingMsg);
      connectionResetFlag = 0;
    }


    _txToggleInstructionMsg();

    // Wait for a character from the serial input
    unsigned char rxChar = _USART_receive_char();

    // Check if the received character is the onChar or offChar
    if (rxChar == _onChar)
    {
        // Turn the LED on
        PORTB |= (1 << PORTB5);
        // Transmit the string "LED turned on!" using USART
        _USART_transmit_string("LED turned on!\r\n");
    }
    
    else if (rxChar == _offChar)
    {
        // Turn the LED off
        PORTB &= ~(1 << PORTB5);
        // Transmit the string "LED turned off!" using USART
        _USART_transmit_string("LED turned off!\r\n");
    }

    else if (rxChar == _exitChar)
    {
      _resetConnection();
    }

    else
    {
        _USART_transmit_string("Char not recognized!\r\n");
    }

    // Print the received character's ASCII code for debugging purposes
    char rxMessage[30];
    sprintf(rxMessage, "Received: %c\r\n\n", rxChar);
    _USART_transmit_string(rxMessage);
}

// ===================================================================
// Private Methods
// ===================================================================

// Function to initialize the USART (Universal Synchronous Asynchronous Receiver Transmitter) communication
void LedController::_USART_init(unsigned int baud)
{

  DDRB |= (1 << DDB5); // Set pin 13 (PB5) as output

  unsigned int ubrr = _calcBaud(baud);
  UBRR0H = (unsigned char)(ubrr >> 8);   // Set the high byte of the USART baud rate register
  UBRR0L = (unsigned char)ubrr;          // Set the low byte of the USART baud rate register

  UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable USART receiver and transmitter
  UCSR0C = (1 << USBS0) | (3 << UCSZ00); // Set USART frame format: 8 data bits, 2 stop bits
}

unsigned int LedController::_calcBaud(unsigned int baud)
{
  return ( F_CPU/ (16UL * baud)) - 1;
  // This ensures that the constant 16 is treated as an unsigned long integer during the calculation. 
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

void LedController::_txToggleInstructionMsg(void)
{
  char msg[30];
  sprintf(msg, "On: %c | Off: %c | Exit: %c \r\n", _onChar, _offChar, _exitChar);
  _USART_transmit_string(msg);
}

void LedController::_resetConnection(void)
{
  // Transmit a message indicating the termination
  _USART_transmit_string("Connection terminated!\r\n");

  // Disable USART communication
  UCSR0B &= ~((1 << RXEN0) | (1 << TXEN0));

  // flush and empty buffer
  while (UCSR0A & (1 << RXC0)) {
      volatile uint8_t dummy = UDR0;
  }

  // Wait for user to disconnect
  _delay_ms(5000);

  // Reset flag 
  _delay_ms(100);
  connectionResetFlag = 1;
}
