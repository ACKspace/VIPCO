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

#define WIDTH 120

//#define DELAY delay(1); 
#define DELAY __asm__("nop\n\t"); 

uint16_t nRows = 0;
int8_t   nCols = 0;

char mode = 'r';
byte a = 0;
byte b = 0;

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

  // Inhibit is active low
  //digitalWrite( INHIBIT, HIGH );

  Serial.begin( 9600 );

  Serial.println( "Initialized." );
  Serial.println( "a: all leds on" );
  Serial.println( "r: random" );
  Serial.println( "b: bars" );
}

void loop()
{
  if ( Serial.available())
  {
    mode = Serial.read();
    uint16_t nVal = Serial.parseInt();
    switch ( mode )
    {
      case 'a':
        for ( byte n = 0; n < 120; n++ )
          shiftBits( 3 );
        strobe();
        digitalWrite( INHIBIT, LOW );      
        break;
        
        case 'r':
          digitalWrite( INHIBIT, LOW );      
        break;
        
        case 'b':
        digitalWrite( INHIBIT, LOW );      
        break;
    }
    
  }

    switch ( mode )
    {
      case 'a':
      for ( byte n = 0; n < 16; n++ )
        selectRow( n );
        break;

      case 'r':
                  for ( byte n = 0; n < 16; n++ )
            {
              for ( byte n = random( 4 ); n > 0; n-- )
              {
                shiftBits( random( 3 ) );
              } 
              strobe();
              selectRow( n );
              delay( 1 );
            }
        break;      
        case 'b':
            if ( !a )
    {
        a = 128;
        b++;

        if ( b > 2 )
        {
          b = 0;
          shiftBits( 3 );
        }
        else
        {
          shiftBits( 0 );
        }
      strobe();
    }

    a--;

    for ( byte n = 0; n < 16; n++ )
    {
      selectRow( n );
    }
  break;
    }
}

void shiftBits( byte _nBits )
{
  digitalWrite( DATA1, (_nBits & 1) ? HIGH : LOW );
  digitalWrite( DATA2, (_nBits & 2) ? HIGH : LOW );
  
  DELAY;
  digitalWrite( CLOCK, HIGH );
  DELAY;
  digitalWrite( CLOCK, LOW );
}

void strobe()
{
  DELAY;
  digitalWrite( STROBE, HIGH );
  DELAY;
  digitalWrite( STROBE, LOW );
}

void selectRow( byte _nRow )
{
  digitalWrite( A, ( _nRow & 1 ) ? HIGH : LOW );
  digitalWrite( B, ( _nRow & 2 ) ? HIGH : LOW );
  digitalWrite( C, ( _nRow & 4 ) ? HIGH : LOW );
  digitalWrite( D, ( _nRow & 8 ) ? HIGH : LOW );
}


