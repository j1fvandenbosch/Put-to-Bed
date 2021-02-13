#include <Arduino.h>
#include <FastLED.h>

#define VERSION 20275

#define  SERIAL

// MATRIX CONFIGURATION - SET THESE BEFORE RUNNING THE TEST

#define MAT_TYPE    NEOPIXEL    // Matrix LED type; see FastLED docs for others
#define MAT_W       8           // Size (columns) of entire matrix i.e. the bottom
#define MAT_H       5           // and rows i.e. the side
#ifdef ESP8266
#define MAT_PIN     0           // Data for matrix on D3 on ESP8266
#else
#define MAT_PIN     13          // Data for matrix on pin 13 for Arduino/other
#endif

#undef  BRIGHTNESS_TEST         // Change "undef" to "define" to show test patterns at startup
#define BRIGHT      32          // brightness; min 0 - 255 max -- high brightness requires a hefty power supply! Start low!

// END OF CONFIGURATION SECTION

CRGB matrix[MAT_H * MAT_W];
uint8_t step;
const uint32_t colors[] = {
    0xff0000,
    0x00ff00,
    0x0000ff,
    0xff00ff,
    0xffff00,
    0x00ffff,
    0xff8000,
    0xffffff
};

void setup() {
  FastLED.addLeds<MAT_TYPE, MAT_PIN>(matrix, MAT_W * MAT_H);
  FastLED.clear();
  FastLED.show();

#ifdef SERIAL
  Serial.begin(115200); while (!Serial) { ; }
  Serial.println();
  Serial.print("PixelTest v"); Serial.println(VERSION);
  Serial.print("Dimensions: "); Serial.print(MAT_W); Serial.print("w x "); Serial.print(MAT_H); Serial.println("h");
  Serial.print("Brightness "); Serial.println(BRIGHT);
#endif

  step = 1;
  randomSeed(millis());
}

void loop() {
  FastLED.clear();
  FastLED.setBrightness(64); // default test brightness
  if ( step == 1 ) {
    // Origin test; light pixel 0 only for 15 seconds, which exposes where the origin
    // of the display is located. If the origin is not in the bottom-left corner of the
    // display, you will need to define either or both MAT_TOP and MAT_LEFT in MatrixFireFast.
    Serial.println("Origin");
    for ( uint8_t i=0; i<5; ++i ) {
      matrix[0] = 0xffffff;
      FastLED.show();
      delay(500);
      matrix[0] = 0;
      FastLED.show();
      delay(500);
    }
  } else if ( step == 2 ) {
    // Row test; light first MAT_W pixels, which shows the procession of pixels. We want
    // a horizontal line the full width of the matrix (row-major ordering). If line is
    // vertical (and possibly appears in more than one column), it's column-major.
    // You need to define MAT_COL_MAJOR in MatrixFireFast.
    Serial.println("Horizontal line");
    for (uint8_t i=0; i<MAT_W; ++i) {
      matrix[i] = colors[i%8];
    }
    FastLED.show();
    delay(5000);
  } else if ( step == 3 ) {
    // Bouncing ball test; should bounce up/down in straight line, no left/right shifting.
    // If there's a left-right shift to the opposite side of the display, then it's zig-zag,
    // and you need to define MAT_ZIGZAG in MatrixFireFast.
    Serial.println("Bouncing ball up/down");
    for ( uint8_t i=0; i<3; ++i ) {
      for ( uint8_t j=0; j<MAT_H; ++j ) {
        matrix[j*MAT_W] = 0xffffff;
        FastLED.show();
        delay(150);
        matrix[j*MAT_W] = 0;
      }
      for ( uint8_t j=MAT_H-2; j>0; --j ) {
        matrix[j*MAT_W] = 0xffffff;
        FastLED.show();
        delay(150);
        matrix[j*MAT_W] = 0;
      }
    }
  } else if ( step == 4 ) {
    // Bouncing ball test II; should bounce left/right in straight line, no up/down shifting.
    // If there's a shift to the opposite edge of the matrix, then it's VERTICAL zig-zag and
    // you need to define MAT_ZIGZAG_VERT in MatrixFireFast.
    Serial.println("Bouncing ball left-right");
    for ( uint8_t i=0; i<3; ++i ) {
      for ( uint8_t j=0; j<MAT_W; ++j ) {
        matrix[j*MAT_H] = 0xffffff;
        FastLED.show();
        delay(150);
        matrix[j*MAT_H] = 0;
      }
      for ( uint8_t j=MAT_W-2; j>0; --j ) {
        matrix[j*MAT_H] = 0xffffff;
        FastLED.show();
        delay(150);
        matrix[j*MAT_H] = 0;
      }
    }
  } else if ( step == 5 ) {
      // Brightness test, for measure worst-case current draw of matrix.
#ifdef BRIGHTNESS_TEST
      Serial.println("Brightness");
      FastLED.setBrightness(BRIGHT);
      for ( uint16_t i=0; i<MAT_H*MAT_W; ++i ) {
        matrix[i] = 0xffffff;
      }
      FastLED.show();
      delay(30000);
#endif
  } else if ( step == 6 ) {
    Serial.println("Sparkle!");
    unsigned long tend = millis() + 5000;
    while ( millis() < tend ) {
      uint16_t k = random(0, MAT_W*MAT_H);
      matrix[k] = 0xffffff;
      FastLED.show();
      delay(15);
      matrix[k] = 0;
    }
  } else {
      Serial.print("End of test cycle"); Serial.println();
      FastLED.show();
      delay(1000);
      step = 0;
  }

  ++step;
}