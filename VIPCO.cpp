/*!
   @file VIPCO.cpp

   @mainpage VIPCO LED Displays

   @section intro_sec Introduction

   This is the documentation for the VIPCO display for the
   Arduino platform.

   See
      https://ackspace.nl/wiki/VIPCO

   This display use a SPI-like interface to communicate, 9 pins are required
   to interface.

   @section dependencies Dependencies

   This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
   Adafruit_GFX</a> being present on your system. Please make sure you have
   installed the latest version before using this library.

   @section author Author

   Written by xopr.

   @section license License

   BSD license, all text here must be included in any redistribution.

*/

#include "VIPCO.h"
#ifndef ARDUINO_STM32_FEATHER
#include "pins_arduino.h"
#ifndef RASPI
#include "wiring_private.h"
#endif
#endif

/**************************************************************************/
/*!
    @brief  Instantiate VIPCO driver
    @param    data1:   data pin 1 (for rows  1-16)
    @param    data2:   data pin 2 (for rows 17-32)
    @param    clk:     clock signal to shift in data pins
    @param    strobe:  signal to latch shift register to LEDs
    @param    a:       row select bit 1
    @param    b:       row select bit 2
    @param    c:       row select bit 3
    @param    d:       row select bit 4
    @param    inhibit: display (row) on
*/
/**************************************************************************/
VIPCO::VIPCO(uint8_t data1, uint8_t data2, uint8_t clk, uint8_t strobe, uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t inhibit)
  : GFXcanvas1(VIPCO_WIDTH, VIPCO_HEIGHT)
{
  m_data1 = data1;
  m_data2 = data2;
  m_clk = clk;
  m_a = a;
  m_b = b;
  m_c = c;
  m_d = d;
  m_inhibit = inhibit;
}

void VIPCO::render(void)
{
  uint8_t* nDisplay = getBuffer();

  for ( byte col = 0; col < 15; col++ )
  {
    byte a = nDisplay[ 15 * m_row + col ];
    byte b = nDisplay[ 240 + 15 * m_row + col ];
    for ( byte pix = 0; pix < 8; pix++ )
    {
      fWriteA( m_data1, ((a >> pix) & 1) ? HIGH : LOW );
      fWriteA( m_data2, ((b >> pix) & 1) ? HIGH : LOW );
      
      fWriteA( m_clk, HIGH );
      fWriteA( m_clk, LOW );
    }
  }
  
  //strobe();
  fWriteA( m_strobe, HIGH );
  fWriteA( m_strobe, LOW );

  //selectRow( row );
  fWriteA( m_a, ( m_row & 1 ) ? HIGH : LOW );
  fWriteA( m_b, ( m_row & 2 ) ? HIGH : LOW );
  fWriteA( m_c, ( m_row & 4 ) ? HIGH : LOW );
  fWriteA( m_d, ( m_row & 8 ) ? HIGH : LOW );
  
  fWriteA( m_inhibit, HIGH );
  fWriteA( m_inhibit, LOW );

  m_row++;
  if ( m_row > 15 )
    m_row = 0;
}
