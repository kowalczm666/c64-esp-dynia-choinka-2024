#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

EspSoftwareSerial::UART testSerial;

Adafruit_NeoPixel strip( 40 , 2, NEO_GRB + /*NEO_KHZ800 */NEO_KHZ400);  //dio-2

void setup() {
  strip.begin();           
  strip.show();            
  strip.setBrightness(10); 
  Serial.begin( 115200 );
  testSerial.begin( 2400, EspSoftwareSerial::SWSERIAL_8N1, 5, 4 ); //  rx-5 tx-4
}

int i = 0;

String rxFrame = "";

// ^Xrrrrgggg..........
// x - należenie 0-9 
void updateLeds( const char *pRxFrame ) {
  int len = strlen(pRxFrame);
  Serial.println(len);
  if (len > 41 ) {
    len = 41;
  }
  int bright = pRxFrame[0] - '0';
  unsigned char brmap[10] = {
    //0   1   2    3    4    5     6     7       8      9
    0,    2,  10,  40,  80,  100,  120,  150,    200,    254
  };
  strip.setBrightness( brmap[ bright ] ); 
  for (int i = 1; i < len; i++ ) {
    switch (pRxFrame[i]) {
      case 'R':
        strip.setPixelColor(i-1, strip.Color(0xff, 0, 0));    
        break;
      case 'G':
        strip.setPixelColor(i-1, strip.Color(0, 0xFF, 0));    
        break;
      case 'B':
        strip.setPixelColor(i-1, strip.Color(0, 0, 0xFF));    
        break;
      case 'Y':
        strip.setPixelColor(i-1, strip.Color(0xFF, 0xFF,0));    
        break;
      case 'W':
        strip.setPixelColor(i-1, strip.Color(0xFF, 0xFF, 0xFF));    
        break;
      default:
        strip.setPixelColor(i-1, strip.Color(0, 0, 0));    
    }    
  }
  strip.show();    
}

void loop() {
  strip.clear();
  while ( 1 ) {
    if ( testSerial.available() > 0 ) {
      char c = testSerial.read();
      if ( c == '\n' ) {
        continue;
      }
      if ( c == '^' ) {
        rxFrame = "";
      }
      else if ( c == '\r') {
        Serial.println (rxFrame );
        updateLeds( rxFrame.c_str() );
        rxFrame = "";
      }
      else {
        rxFrame += c;
        if ( rxFrame.length() > 64 ) {
          rxFrame = "";
        }
      }
    }
  }
}
