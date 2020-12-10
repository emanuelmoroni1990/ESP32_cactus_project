/*  
 http://becomingmaker.com/usi-serial-send-attiny/
 Author: Mark Osborne, BecomingMaker.com
 See accompanying post http://becomingmaker.com/
 
 An example of USI Serial Send for ATtiny25/45/85.
 Sends a text message every second.
   
  ATTiny85 Hookup
  RESET -|1 v 8|- Vcc
    PB3 -|2   7|- PB2/SCK
    PB4 -|3   6|- PB1/MISO/DO
    GND -|4 _ 5|- PB0/MOSI/SDA
ATTiny85 PB1/MISO/DO = Serial UART Tx -> connect to Rx of serial output device
*/

/* Supported combinations:
 *   F_CPU 1000000   BAUDRATE 1200, 2400 
 *   F_CPU 8000000   BAUDRATE 9600, 19200
 *   F_CPU 16000000  BAUDRATE 9600, 19200, 28800, 38400
 */

#include <Arduino.h>

// Set your baud rate and number of stop bits here
#define BAUDRATE            9600
#define STOPBITS            1
//F_CPU defined by Arduino, e.g. 1000000, 8000000, 16000000
//#define F_CPU 8000000

// If bit width in cpu cycles is greater than 255 then  divide by 8 to fit in timer
// Calculate prescaler setting
#define CYCLES_PER_BIT       ( (F_CPU) / (BAUDRATE) )
#if (CYCLES_PER_BIT > 255)
#define DIVISOR             8
#define CLOCKSELECT         2
#else
#define DIVISOR             1
#define CLOCKSELECT         1
#endif
#define FULL_BIT_TICKS      ( (CYCLES_PER_BIT) / (DIVISOR) )

// Old timer values
#ifdef ARDUINO
volatile static uint8_t oldTCCR0A;
volatile static uint8_t oldTCCR0B;
volatile static uint8_t oldTCNT0;
#endif

// USISerial send state variable and accessors
enum USISERIAL_SEND_STATE { AVAILABLE, FIRST, SECOND };
static volatile enum USISERIAL_SEND_STATE usiserial_send_state = AVAILABLE;
static inline enum USISERIAL_SEND_STATE usiserial_send_get_state(void)
{
    return usiserial_send_state;
}
static inline void usiserial_send_set_state(enum USISERIAL_SEND_STATE state)
{
    usiserial_send_state=state;
}

// Transmit data persistent between USI OVF interrupts
static volatile uint8_t usiserial_tx_data;
static inline uint8_t usiserial_get_tx_data(void)
{
    return usiserial_tx_data;
}
static inline void usiserial_set_tx_data(uint8_t tx_data)
{
    usiserial_tx_data = tx_data;
}

bool usiserial_send_available ();
static uint8_t reverse_byte (uint8_t);
void usiserial_send_byte (uint8_t);