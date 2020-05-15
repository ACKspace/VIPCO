#include "VIPCO.h"
#include <Fonts/FreeSerif12pt7b.h>
/*
back side (top right)
----------+
    VCC oo|data2
    VCC oo|VCC
  clock oo|D
 strobe oo|C
  data1 oo|B
inhibit oo|A
    GND oo|GND
    GND oo|GND
*/

#define DATA1 4
#define DATA2 6
#define CLOCK 2
#define STROBE 3

#define A 10
#define B 9
#define C 8
#define D 7
#define INHIBIT 5

VIPCO vipco = VIPCO( DATA1, DATA2, CLOCK, STROBE, A, B, C, D, INHIBIT );

volatile uint8_t g_row = 0;

// Timer0 compare interrupt that piggybacks on the arduino interrupt every ms
SIGNAL(TIMER0_COMPA_vect) 
{
  uint8_t* nDisplay = vipco.getBuffer();

  // NOTE:
  // Tried to call `vipco.render()` which didn't work and/or perform as expected
  // Tried to use portOutputRegister(digitalPinToPort(pin)) and digitalPinToBitMask(pin)
  //    which didn'twork (when not using volatile) and didn't perform otherwise

  // ALSO NOTE:
  // `fWriteA()` is defined in VIPCO.h; ultimately, the class instance `render()` should be called
  fWriteA( INHIBIT, LOW );

  for ( byte col = 0; col < 15; col++ )
  {
    byte a = nDisplay[ 15 * g_row + 14 - col ];
    byte b = nDisplay[ 240 + 15 * g_row + 14 - col ];
    for ( byte pix = 0; pix < 8; pix++ )
    {
      fWriteA( DATA1, ((a >> (7 - pix)) & 1) ? HIGH : LOW );
      fWriteA( DATA2, ((b >> (7 - pix)) & 1) ? HIGH : LOW );

      fWriteA( CLOCK, HIGH );
      fWriteA( CLOCK, LOW );
    }
  }
  
  fWriteA( STROBE, HIGH );
  fWriteA( STROBE, LOW );

  fWriteA( A, ( g_row & 1 ) ? HIGH : LOW );
  fWriteA( B, ( g_row & 2 ) ? HIGH : LOW );
  fWriteA( C, ( g_row & 4 ) ? HIGH : LOW );
  fWriteA( D, ( g_row & 8 ) ? HIGH : LOW );

  fWriteA( INHIBIT, HIGH );

  g_row++;
  if ( g_row > 15 )
    g_row = 0;
}

void setup()
{
  pinMode( DATA1, OUTPUT );
  pinMode( DATA2, OUTPUT );
  pinMode( CLOCK, OUTPUT );
  pinMode( STROBE, OUTPUT );
  
  pinMode( A, OUTPUT );
  pinMode( B, OUTPUT );
  pinMode( C, OUTPUT );
  pinMode( D, OUTPUT );
  pinMode( INHIBIT, OUTPUT );

  Serial.begin( 9600 );
  Serial.println( "Initialized." );

  // 0=upside down
  // 1=counter clockwise
  // 2=upright
  // 3=clockwise
  vipco.setRotation( 1 );
  vipco.setFont( &FreeSerif12pt7b );
  
  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

uint8_t _a = 0;
uint8_t _c = 0;
uint8_t k = 0;
uint8_t s = 0;
uint8_t p = 0;
uint8_t a = 0;
uint8_t c = 0;
uint8_t e = 0;

// Simple sine table, 15 degrees steps from -7 to 7
int8_t sine[] =
{
  0,2,4,5,6,7,7,7,6,5,4,2,0,-2,-4,-5,-6,-7,-7,-7,-6,-5,-4,-2,0
};

void loop()
{
  uint8_t* nDisplay = vipco.getBuffer();

  uint8_t pos = (millis() >> 7 );
  
  vipco.drawChar(_a, 16, 'A', 0, 0, 1 );
  _a = 7 + sine[ (pos + 14) % 26 ];
  vipco.drawChar(_a, 16, 'A', 1, 0, 1 );
  
  vipco.drawChar(_c, 33, 'C', 0, 0, 1 );
  _c = 7 + sine[ (pos + 12) % 26 ];
  vipco.drawChar(_c, 33, 'C', 1, 0, 1 );
    
  vipco.drawChar(k, 50, 'K', 0, 0, 1 );
  k = 7 + sine[ (pos + 10) % 26 ];
  vipco.drawChar(k, 50, 'K', 1, 0, 1 );

  vipco.drawChar(s, 62, 's', 0, 0, 1 );
  s = 11 + sine[ (pos + 8) % 26 ];
  vipco.drawChar(s, 62, 's', 1, 0, 1 );

  vipco.drawChar(p, 74, 'p', 0, 0, 1 );
  p = 9 + sine[ (pos + 6) % 26 ];
  vipco.drawChar(p, 74, 'p', 1, 0, 1 );

  vipco.drawChar(a, 91, 'a', 0, 0, 1 );
  a = 9 + sine[ (pos + 4) % 26 ];
  vipco.drawChar(a, 91, 'a', 1, 0, 1 );

  vipco.drawChar(c, 103, 'c', 0, 0, 1 );
  c = 9 + sine[ (pos + 2) % 26 ];
  vipco.drawChar(c, 103, 'c', 1, 0, 1 );

  vipco.drawChar(e, 115, 'e', 0, 0, 1 );
  e = 9 + sine[ (pos + 0) % 26 ];
  vipco.drawChar(e, 115, 'e', 1, 0, 1 );

  if ( Serial.available() )
    vipco.write( Serial.read() );
}
