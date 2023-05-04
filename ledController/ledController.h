#ifndef LEDCONTROLLER
#define LEDCONTROLLER

// @brief: Led Class Controller class for turning on/off the onboard LED of Arduino Nano via Serial communication
class LedController
{
  public:

    LedController(const int& baudRate, char onChar, char offChar) : _onChar(onChar), _offChar(offChar)
    {
      _USART_init(baudRate);
    };

    void setGreetingMsg(const char*);

    void run(void);

    ~LedController() = default;

  private:

    const char _onChar;
    const char _offChar;
    const char* _greetingMsg;

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
};

#endif


