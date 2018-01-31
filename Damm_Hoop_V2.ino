// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "Beam.cpp";

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN 6
#define ARR_SIZE 3
#define ARR_MAX_SIZE 3

#define GLOBAL_DELAY 5

//#define GLOBAL_SPEED 1
#define GLOBAL_SPEED_TOP 3
#define GLOBAL_SPEED_BOTTOM 1

#define GLOBAL_SIZE_BOTTOM 5
#define GLOBAL_SIZE_TOP 20

#define GLOBAL_LIFE_BOTTOM 250
#define GLOBAL_LIFE_TOP 400

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 209

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

Beam arrBeam[ARR_MAX_SIZE] = {
  Beam(int(random(GLOBAL_SIZE_BOTTOM,GLOBAL_SIZE_TOP)), GLOBAL_SPEED_BOTTOM, random(GLOBAL_LIFE_BOTTOM,GLOBAL_LIFE_TOP), 1, 0, NUMPIXELS),
  Beam(int(random(GLOBAL_SIZE_BOTTOM,GLOBAL_SIZE_TOP)), GLOBAL_SPEED_BOTTOM, random(GLOBAL_LIFE_BOTTOM,GLOBAL_LIFE_TOP), 1, 65, NUMPIXELS),
  Beam(int(random(GLOBAL_SIZE_BOTTOM,GLOBAL_SIZE_TOP)), GLOBAL_SPEED_BOTTOM, random(GLOBAL_LIFE_BOTTOM,GLOBAL_LIFE_TOP), 1, 130, NUMPIXELS)
  //Beam(int(random(10,30)), random(.03,.3), random(.013,.02),1, int(random(0,NUMPIXELS-1)), NUMPIXELS),
  //Beam(int(random(10,30)), random(.03,.3), random(.013,.02),1, int(random(0,NUMPIXELS-1)), NUMPIXELS)
}; // = new ArrayList<Beam>();


/*
 * Array of Beam objects
 */

void setup() {

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(9600);
  Serial.println(">>Hello");
  
  pixels.begin(); // This initializes the NeoPixel library.
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0,255));
  }
  pixels.show();

}

void loop() {

  //Serial.println("1");
  /*
   * If any Beams ready for removal, remove and add a new one
   */
  for (int i=0; i<ARR_SIZE; i++) {

    if(arrBeam[i].move() != 1) {

      arrBeam[i].reset(int(random(GLOBAL_SIZE_BOTTOM,GLOBAL_SIZE_TOP)), random(GLOBAL_SPEED_BOTTOM, GLOBAL_SPEED_TOP)/10.0, random(GLOBAL_LIFE_BOTTOM,GLOBAL_LIFE_TOP), 1, random(0,NUMPIXELS), NUMPIXELS);      
    }
  }

  /*
   * Set correct color, then show the ring
   */
  float _val = 0;

  for(int i=0; i<NUMPIXELS; i++){

    _val = 0;
    for (int j=0; j<ARR_SIZE; j++) {
      
      _val += arrBeam[j].getValBright(i) * 255.0;
    }

    if (_val > 255) {
      _val = 255;
    }

    if (_val < 0) { _val = 0;}
    pixels.setPixelColor(i, pixels.Color(0,0,0,_val));

  }

  pixels.show();
  delay(GLOBAL_DELAY);

}
