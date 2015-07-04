#include "FastLED.h"

#define LED_PIN     4
#define NUM_LEDS    24
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 25

#define TEMPERATURE_1 HighNoonSun

const byte buttonPin = 0;
const byte max_color = 6;
byte cur_color = 5;
byte lastButtonState = 1; // previous state of the button
byte buttonState = 1;
byte slowColor = 5;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  //clock_prescale_set(clock_div_1);
  pinMode(buttonPin, INPUT_PULLUP);
  //Serial.begin(9600);
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setTemperature( TEMPERATURE_1 );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}


void loop()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      // button push brings pin low, if this is the case then switch colors
      if (buttonState == LOW) {
        cur_color++;
        if (cur_color > max_color) {
          cur_color = 1;
        }
      }
    }
  }
  ChangePalette();

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);
  lastButtonState = reading;

  FastLED.show();
  // if the slowColor is selected, randomize the delay to appear as flickering
  if (cur_color == slowColor) {
    int randomDelay = random(2,100);
    //Serial.println(randomDelay);
    FastLED.delay(1500 / randomDelay);
  } else {
    FastLED.delay(1000 / UPDATES_PER_SECOND);
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  int value;
  // If the slowColor is selected (warm white), randomize the fill
  // so that it looks more like the flickering of a lantern
  for( int i = 0; i < NUM_LEDS; i++) {
    if (cur_color == slowColor) {
      value = random(i, NUM_LEDS);
    } else {
      value = i;
    }
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    if (cur_color != 4) {
      colorIndex += 3;
    }
  }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalette()
{
    switch (cur_color){
      case 1:

        currentBlending = LINEARBLEND;
        currentPalette = LavaColors_p;
        break;
      case 2:
        currentPalette = ForestColors_p;
        break;
      case 3:
        currentPalette = OceanColors_p;
        break;
      case 4:
        currentPalette = RainbowColors_p;
        break;
      case 5:
        // warm white
        warmWhitePalette();
        break;
      case 6:
        // bright white
        whitePalette();
        break;
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void warmWhitePalette()
{
  // 'black out' all 16 palette entries...
  static CHSV hsvwarmWhite = CHSV( 45, 175, 255);
  static CRGB black = CRGB::Black;
  static CRGB yellow = CHSV( 45, 255,255);
  static CRGB red = CHSV( HUE_RED, 255, 255);
  //fill_solid( currentPalette, 16, hsvwarmWhite);
  currentPalette = CRGBPalette16(
    hsvwarmWhite,  hsvwarmWhite,  hsvwarmWhite,  yellow,
    yellow, red, black,  yellow,
    hsvwarmWhite,  hsvwarmWhite,  black,  hsvwarmWhite,
    hsvwarmWhite, hsvwarmWhite, yellow,  hsvwarmWhite );

}

void whitePalette()
{
  // 'black out' all 16 palette entries...
  //CRGB white = CRGB(255,255,255);
  fill_solid( currentPalette, 16, CRGB(255,255,255));

}
