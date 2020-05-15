/*!
 * @file VIPCO.h
 *
 * This is the documentation for the VIPCO display for the
 * Arduino platform.
 *
 * See
 *    https://ackspace.nl/wiki/VIPCO
 *
 * This display use a SPI-like interface to communicate, 9 pins are required
 * to interface.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef _VIPCOH_
#define _VIPCOH_

#include "Adafruit_GFX.h"
#include "Arduino.h"
#include "Print.h"

#define VIPCO_WIDTH 120  ///< VIPCO max width
#define VIPCO_HEIGHT 32  ///< VIPCO max height

//#define USE_FAST_PINIO

//Arduino MEGA compatible verison of fastWrite
#ifdef ARDUINO_AVR_UNO
#define fWriteA(pin, val) if (val) switch (pin) {     \
   case 0: PORTD |= 1<<0; break;       \
   case 1: PORTD |= 1<<1; break;       \
   case 2: PORTD |= 1<<2; break;       \
   case 3: PORTD |= 1<<3; break;       \
   case 4: PORTD |= 1<<4; break;       \
   case 5: PORTD |= 1<<5; break;       \
   case 6: PORTD |= 1<<6; break;       \
   case 7: PORTD |= 1<<7; break;       \
   case 8: PORTB |= 1<<0; break;       \
   case 9: PORTB |= 1<<1; break;       \
   case 10: PORTB |= 1<<2; break;      \
   case 11: PORTB |= 1<<3; break;      \
   case 12: PORTB |= 1<<4; break;      \
   case 13: PORTB |= 1<<5; break;      \
   } else switch (pin) {               \
   case 0: PORTD &= ~(1<<0); break;    \
   case 1: PORTD &= ~(1<<1); break;    \
   case 2: PORTD &= ~(1<<2); break;    \
   case 3: PORTD &= ~(1<<3); break;    \
   case 4: PORTD &= ~(1<<4); break;    \
   case 5: PORTD &= ~(1<<5); break;    \
   case 6: PORTD &= ~(1<<6); break;    \
   case 7: PORTD &= ~(1<<7); break;    \
   case 8: PORTB &= ~(1<<0); break;    \
   case 9: PORTB &= ~(1<<1); break;    \
   case 10: PORTB &= ~(1<<2); break;   \
   case 11: PORTB &= ~(1<<3); break;   \
   case 12: PORTB &= ~(1<<4); break;   \
   case 13: PORTB &= ~(1<<5); break;   \
   }
 #endif

#if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
#define fWriteA(pin, val) if (val) switch (pin) {     \
    case 0: PORTE |= 1<<0; break;       \
    case 1: PORTE |= 1<<1; break;       \
    case 2: PORTE |= 1<<4; break;       \
    case 3: PORTE |= 1<<5; break;       \
    case 4: PORTG |= 1<<5; break;       \
    case 5: PORTE |= 1<<3; break;       \
    case 6: PORTH |= 1<<3; break;       \
    case 7: PORTH |= 1<<4; break;       \
    case 8: PORTH |= 1<<5; break;       \
    case 9: PORTH |= 1<<6; break;       \
    case 10: PORTB |= 1<<4; break;      \
    case 11: PORTB |= 1<<5; break;      \
    case 12: PORTB |= 1<<6; break;      \
    case 13: PORTB |= 1<<7; break;      \
    } else switch (pin) {               \
    case 0: PORTE &= ~(1<<0); break;    \
    case 1: PORTE &= ~(1<<1); break;    \
    case 2: PORTE &= ~(1<<4); break;    \
    case 3: PORTE &= ~(1<<5); break;    \
    case 4: PORTG &= ~(1<<5); break;    \
    case 5: PORTE &= ~(1<<3); break;    \
    case 6: PORTH &= ~(1<<3); break;    \
    case 7: PORTH &= ~(1<<4); break;    \
    case 8: PORTH &= ~(1<<5); break;    \
    case 9: PORTH &= ~(1<<6); break;    \
    case 10: PORTB &= ~(1<<4); break;   \
    case 11: PORTB &= ~(1<<5); break;   \
    case 12: PORTB &= ~(1<<6); break;   \
    case 13: PORTB &= ~(1<<7); break;   \
}
#endif

#ifdef ARDUINO_AVR_LEONARD
#endif

/**************************************************************************/
/*!
@brief Class to manage hardware interface with VIPCO 
*/
/**************************************************************************/
class VIPCO : public GFXcanvas1 {
private:
  uint8_t m_data1;
  uint8_t m_data2;
  uint8_t m_clk;
  uint8_t m_strobe;
  uint8_t m_a;
  uint8_t m_b;
  uint8_t m_c;
  uint8_t m_d;
  uint8_t m_inhibit;
  
public:
  VIPCO(uint8_t _DATA1, uint8_t _DATA2, uint8_t _CLK, uint8_t _STROBE, uint8_t _A, uint8_t _B, uint8_t _C, uint8_t _D, uint8_t _INHIBIT);

  void render(void);

  //void drawPixel(int16_t x, int16_t y, uint16_t color);
  //void setRotation(uint8_t r);
private:
  volatile uint8_t m_row;
};

#endif // _VIPCOH_
