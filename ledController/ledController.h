#ifndef LEDCONTROLLER
#define LEDCONTROLLER

// @brief: Led Class Controller class for turning on/off the onboard LED of Arduino Nano via Serial communication
class LedController
{
  public:

    LedController(unsigned int baudRate, char onChar, char offChar)
      : _baud(baudRate), _onChar(onChar), _offChar(offChar){};

    void setGreetingMsg(const char* msg);

    void run(void);

    ~LedController() = default;

  private:

    const char _onChar;
    const char _offChar;
    const char _exitChar = 'x';
    const char* _greetingMsg;
    const unsigned int _baud;

    // Function to initialize the USART (Universal Synchronous Asynchronous Receiver Transmitter) communication
    void _USART_init(unsigned int baud);

    // Function to transmit a character using USART
    void _USART_transmit_char(unsigned char data);

    // Function to transmit a string using USART
    void _USART_transmit_string(const char *str);

    // Function to receive a character using USART
    unsigned char _USART_receive_char(void);

    // Calculate USART baud register value from Baud rate
    unsigned int _calcBaud(unsigned int baud);

    // Send the toggle keys with every msg
    void _txToggleInstructionMsg(void);

    // Terminate connectiona and then reset after 3 sec
    void _resetConnection(void);
};

#endif


