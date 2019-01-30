#include <FastLED.h>
#include <Thread.h>
#include <ThreadController.h>

#define CIRCLE_PIN         9
#define CIRCLE_NUM_LEDS    63

#define OUTSQ_PIN          10
#define OUTSQ_NUM_LEDS     46

#define INSQ_PIN           11
#define INSQ_NUM_LEDS      46

#define EYE_PIN            3
#define EYE_NUM_LEDS       18

#define BRIGHTNESS         255

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB


CRGB lcir [CIRCLE_NUM_LEDS];
CRGB lout [ OUTSQ_NUM_LEDS];
CRGB lin  [ INSQ_NUM_LEDS ];
CRGB leye [ EYE_NUM_LEDS  ];

void clearPixels() {
  fill_solid(leye, EYE_NUM_LEDS, CRGB::Black);
  fill_solid(lin,  INSQ_NUM_LEDS, CRGB::Black);
  fill_solid(lout, OUTSQ_NUM_LEDS, CRGB::Black);
  fill_solid(lcir, CIRCLE_NUM_LEDS, CRGB::Black);
  FastLED.show();
}

// ------------
// ------------
// ------------

void eyeAni() {
    Serial.println("Eye");
  
    static byte eb = 0;

    switch(eb) {
      case 0:
        for (byte i = EYE_NUM_LEDS/2; i < EYE_NUM_LEDS; i++)
          leye[i] = CRGB::Black;
        break;

      case 1:
        for (byte i = 0; i < EYE_NUM_LEDS/2; i++)
          leye[i] = CRGB::Orange;
        break;
      
      case 2:
        for (byte i = EYE_NUM_LEDS/2; i < EYE_NUM_LEDS; i++)
          leye[i] = CRGB::Orange;
        break;

      case 3:
        for (byte i = 0; i < EYE_NUM_LEDS/2; i++)
            leye[i] = CRGB::Black;
        break;
    }

    eb = (eb + 1) % 4;
    delay(50);
    FastLED.show(); 

    Serial.println("Eye End");
}

#define LED_TRAIN 5
void inAni() {
    Serial.println("InSquare");
  
    for (byte q = 0; q < INSQ_NUM_LEDS; q = q + LED_TRAIN) {
      for (byte i = 0; i < LED_TRAIN; i++) {
        lin [i+q] = CRGB::Red;    //turn every third pixel on  
      }

      FastLED.show();
      delay(200);

      for (byte i = 0; i < LED_TRAIN; i++) {
        lin [i+q] = CRGB::Black;    //turn every third pixel on  
      }
    }
  
    delay(200); 

    Serial.println("InSquare End");
}

//void outAni() {
//  static byte position = 0;
//  
//  for (uint8_t j = 0; j < OUTSQ_NUM_LEDS; j+=2)
//    lout[j] = CRGB::Orange;
//
//  FastLED.show();
//  fill_solid(lout, OUTSQ_NUM_LEDS, CRGB::Black);
//
//  i = (i + 1) % 2;
//}

void cirAni() {
    Serial.println("Circle");
  
    for (byte i = 0; i < CIRCLE_NUM_LEDS; i++) {
      lcir[i] = CRGB::Orange;
      FastLED.show();
      delay(100 - i);
    }

    for (byte i = 0; i < CIRCLE_NUM_LEDS; i++) {
      lcir[i] = CRGB::Black;
      FastLED.show();
      delay(100 - i);
    }

    Serial.println("Circle End");
}

// ------------
// ------------
// ------------

ThreadController tc = ThreadController();

void setup() {
  Serial.begin(9600);
  delay( 100 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, CIRCLE_PIN, COLOR_ORDER>(lcir, CIRCLE_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, OUTSQ_PIN, COLOR_ORDER>(lout, OUTSQ_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, INSQ_PIN, COLOR_ORDER>(lin, INSQ_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, EYE_PIN, COLOR_ORDER>(leye, EYE_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  clearPixels();

  Thread eyeT = Thread(); eyeT.onRun(eyeAni);
//  Thread outT = Thread(); outT.onRun(outAni);
  Thread inT = Thread() ;  inT.onRun(inAni) ;
  Thread cirT = Thread(); cirT.onRun(cirAni);

  tc.add(&eyeT);
  tc.add(&cirT);
//  tc.add(&outT);
  tc.add(&inT);
}

// ------------
// ------------
// ------------

void loop(){
  tc.run();  
}
