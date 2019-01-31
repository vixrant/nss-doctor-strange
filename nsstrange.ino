#include <FastLED.h>

#define CIRCLE_PIN 9
#define CIR_NUM 63

#define OUTSQ_PIN 10
#define OUT_NUM 45

#define INSQ_PIN 11
#define IN_NUM 46

#define EYE_PIN 3
#define EYE_NUM 18

#define BRIGHTNESS 255

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define COLOR_MAIN CRGB::Gold

// For wipes
#define FORWARD true
#define BACKWARD false

CRGB lcir[CIR_NUM];
CRGB lout[OUT_NUM];
CRGB lin[IN_NUM];
CRGB leye[EYE_NUM];

void clearPixels()
{
	fill_solid(leye, EYE_NUM, CRGB::Black);
	fill_solid(lin, IN_NUM, CRGB::Black);
	fill_solid(lout, OUT_NUM, CRGB::Black);
	fill_solid(lcir, CIR_NUM, CRGB::Black);
	FastLED.show();
}

// ------------
// ------------
// ------------

unsigned long int eyeTimer = 0;
void eyeAni()
{
	Serial.println("Eye");
	Serial.println(millis());

	static byte eb = 0;

	switch (eb)
	{
	case 0:
//		for (byte i = EYE_NUM / 2; i < EYE_NUM; i++)
//			leye[i] = CRGB::Black;
		break;

	case 1:
		for (byte i = 0; i < EYE_NUM / 2; i++)
			leye[i] = CRGB::Green;
		break;

	case 2:
		for (byte i = EYE_NUM / 2; i < EYE_NUM; i++)
			leye[i] = CRGB::Green;
		break;

	case 3:
		        for (byte i = 0; i < EYE_NUM/2; i++)
		            leye[i] = CRGB::Black;
		break;
	}

	eb = (eb + 1) % 4;
	FastLED.show();

	eyeTimer = millis();
	Serial.println("Eye End ");
	Serial.println(eyeTimer);
}

#define MAX_JUMPS 5
unsigned long inTimer = 0;

void inAni()
{
	static boolean direction = BACKWARD;
	static byte i = 0;

	if (i == 0)
	{
		direction = !direction;
	}

	lin[i] = direction == FORWARD ? COLOR_MAIN : CRGB::Black;
	FastLED.show();

	i = (i + 1) % IN_NUM;

	inTimer = millis();
}

unsigned long outTimer = 0;
void outAni()
{
	static boolean direction = BACKWARD;
	static byte i = 0;

	if (i == 0)
	{
		direction = !direction;
	}

	lout[i] = direction == FORWARD ? COLOR_MAIN : CRGB::Black;
	FastLED.show();

	i = (i + 1) % OUT_NUM;

	outTimer = millis();
}

boolean CIR_GO = true;
unsigned long int cirTimer = 0;
unsigned long int goTimer = 0;
void cirAni()
{
	static boolean direction = BACKWARD;
	static byte i = 0;

	if (i == 0)
	{
		direction = !direction;
    CIR_GO = false;
    goTimer = millis();
	}

	lcir[i] = direction == FORWARD ? COLOR_MAIN : CRGB::Black;
	FastLED.show();

	i = (i + 1) % CIR_NUM;

	cirTimer = millis();
}

// ------------
// ------------
// ------------

void setup()
{
	delay(100); // power-up safety delay
	FastLED.addLeds<LED_TYPE, CIRCLE_PIN, COLOR_ORDER>(lcir, CIR_NUM).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE, OUTSQ_PIN, COLOR_ORDER>(lout, OUT_NUM).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE, INSQ_PIN, COLOR_ORDER>(lin, IN_NUM).setCorrection(TypicalLEDStrip);
	FastLED.addLeds<LED_TYPE, EYE_PIN, COLOR_ORDER>(leye, EYE_NUM).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(BRIGHTNESS);

	clearPixels();
}

// ------------
// ------------
// ------------

#define EYE_INTERVAL 500
#define IN_INTERVAL 50
#define OUT_INTERVAL 35
#define CIR_INTERVAL 25
#define CIR_GO_INT  2000
void loop()
{
	const unsigned long int currentTime = millis();
 if (currentTime - goTimer >= CIR_GO_INT && !CIR_GO)
     CIR_GO = true;
	if (currentTime - eyeTimer >= EYE_INTERVAL)
		eyeAni();
	if (currentTime - cirTimer >= CIR_INTERVAL)
		cirAni();
	if (currentTime - inTimer >= IN_INTERVAL)
		inAni();
	if (currentTime - outTimer >= OUT_INTERVAL)
		outAni();

}
