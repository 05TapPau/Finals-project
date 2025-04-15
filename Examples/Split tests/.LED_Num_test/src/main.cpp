#include <Arduino.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 80 // technically only 64 but extra 16 because of spacing and continuation of the loop past me knows what im doing trust

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

int LED_POS = 0;



void fadeall()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i].nscale8(250);
  }
}
void Startupanimation()
{
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for (int i = 0; i < NUM_LEDS; i++)
  {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    delay(10);
  }

  // Now go in the other direction.
  for (int i = (NUM_LEDS)-1; i >= 0; i--)
  {
    // Set the i'th led to red
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    delay(10);
  }
  FastLED.clear();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("resetting");
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(25);
  Startupanimation();
}

void WriteZero()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 0);
  leds[LED_POS + 12] = CHSV(85, 255, 0);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 0);
  leds[LED_POS + 20] = CHSV(85, 255, 0);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 255);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 255);
  leds[LED_POS + 30] = CHSV(85, 255, 255);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}
void WriteOne()
{
  leds[LED_POS] = CHSV(85, 255, 0);
  leds[LED_POS + 1] = CHSV(85, 255, 0);
  leds[LED_POS + 2] = CHSV(85, 255, 0);
  leds[LED_POS + 3] = CHSV(85, 255, 0);
  leds[LED_POS + 4] = CHSV(85, 255, 0);
  leds[LED_POS + 5] = CHSV(85, 255, 0);
  leds[LED_POS + 6] = CHSV(85, 255, 0);
  leds[LED_POS + 7] = CHSV(85, 255, 0);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 255);
  leds[LED_POS + 10] = CHSV(85, 255, 255);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 255);
  leds[LED_POS + 14] = CHSV(85, 255, 255);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 255);
  leds[LED_POS + 18] = CHSV(85, 255, 255);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 255);
  leds[LED_POS + 22] = CHSV(85, 255, 255);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 0);
  leds[LED_POS + 25] = CHSV(85, 255, 0);
  leds[LED_POS + 26] = CHSV(85, 255, 0);
  leds[LED_POS + 27] = CHSV(85, 255, 0);
  leds[LED_POS + 28] = CHSV(85, 255, 0);
  leds[LED_POS + 29] = CHSV(85, 255, 0);
  leds[LED_POS + 30] = CHSV(85, 255, 0);
  leds[LED_POS + 31] = CHSV(85, 255, 0);
}
void WriteTwo()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 0);
  leds[LED_POS + 2] = CHSV(85, 255, 0);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 0);
  leds[LED_POS + 26] = CHSV(85, 255, 0);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 255);
  leds[LED_POS + 30] = CHSV(85, 255, 255);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}
void WriteThree()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 0);
  leds[LED_POS + 26] = CHSV(85, 255, 0);
  leds[LED_POS + 27] = CHSV(85, 255, 0);
  leds[LED_POS + 28] = CHSV(85, 255, 0);
  leds[LED_POS + 29] = CHSV(85, 255, 0);
  leds[LED_POS + 30] = CHSV(85, 255, 0);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}
void WriteFour()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 0);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 0);
  leds[LED_POS + 16] = CHSV(85, 255, 0);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 0);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 255);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 0);
  leds[LED_POS + 30] = CHSV(85, 255, 0);
  leds[LED_POS + 31] = CHSV(85, 255, 0);
}
void WriteFive()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 0);
  leds[LED_POS + 6] = CHSV(85, 255, 0);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 255);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 0);
  leds[LED_POS + 30] = CHSV(85, 255, 0);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}
void WriteSix()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 0);
  leds[LED_POS + 6] = CHSV(85, 255, 0);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 255);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 255);
  leds[LED_POS + 30] = CHSV(85, 255, 255);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}
void WriteSeven()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 255);
  leds[LED_POS + 10] = CHSV(85, 255, 255);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 255);
  leds[LED_POS + 14] = CHSV(85, 255, 255);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 0);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 0);
  leds[LED_POS + 20] = CHSV(85, 255, 0);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 255);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 0);
  leds[LED_POS + 27] = CHSV(85, 255, 0);
  leds[LED_POS + 28] = CHSV(85, 255, 0);
  leds[LED_POS + 29] = CHSV(85, 255, 0);
  leds[LED_POS + 30] = CHSV(85, 255, 0);
  leds[LED_POS + 31] = CHSV(85, 255, 0);
}
void WriteEight()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 255);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 255);
  leds[LED_POS + 30] = CHSV(85, 255, 255);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}
void WriteNine()
{
  leds[LED_POS] = CHSV(85, 255, 255);
  leds[LED_POS + 1] = CHSV(85, 255, 255);
  leds[LED_POS + 2] = CHSV(85, 255, 255);
  leds[LED_POS + 3] = CHSV(85, 255, 255);
  leds[LED_POS + 4] = CHSV(85, 255, 255);
  leds[LED_POS + 5] = CHSV(85, 255, 255);
  leds[LED_POS + 6] = CHSV(85, 255, 255);
  leds[LED_POS + 7] = CHSV(85, 255, 255);
  leds[LED_POS + 8] = CHSV(85, 255, 255);
  leds[LED_POS + 9] = CHSV(85, 255, 0);
  leds[LED_POS + 10] = CHSV(85, 255, 0);
  leds[LED_POS + 11] = CHSV(85, 255, 255);
  leds[LED_POS + 12] = CHSV(85, 255, 255);
  leds[LED_POS + 13] = CHSV(85, 255, 0);
  leds[LED_POS + 14] = CHSV(85, 255, 0);
  leds[LED_POS + 15] = CHSV(85, 255, 255);
  leds[LED_POS + 16] = CHSV(85, 255, 255);
  leds[LED_POS + 17] = CHSV(85, 255, 0);
  leds[LED_POS + 18] = CHSV(85, 255, 0);
  leds[LED_POS + 19] = CHSV(85, 255, 255);
  leds[LED_POS + 20] = CHSV(85, 255, 255);
  leds[LED_POS + 21] = CHSV(85, 255, 0);
  leds[LED_POS + 22] = CHSV(85, 255, 0);
  leds[LED_POS + 23] = CHSV(85, 255, 255);
  leds[LED_POS + 24] = CHSV(85, 255, 255);
  leds[LED_POS + 25] = CHSV(85, 255, 255);
  leds[LED_POS + 26] = CHSV(85, 255, 255);
  leds[LED_POS + 27] = CHSV(85, 255, 255);
  leds[LED_POS + 28] = CHSV(85, 255, 255);
  leds[LED_POS + 29] = CHSV(85, 255, 0);
  leds[LED_POS + 30] = CHSV(85, 255, 0);
  leds[LED_POS + 31] = CHSV(85, 255, 255);
}

int counter = 0;
int Ones = 0;
int tens = 0;

void writeNum()
{
  LED_POS = 0;
  switch (Ones)
  {
  case 0:
    WriteZero();
    break;
  case 1:
    WriteOne();
    break;
  case 2:
    WriteTwo();
    break;
  case 3:
    WriteThree();
    break;
  case 4:
    WriteFour();
    break;
  case 5:
    WriteFive();
    break;
  case 6:
    WriteSix();
    break;
  case 7:
    WriteSeven();
    break;
  case 8:
    WriteEight();
    break;
  case 9:
    WriteNine();
    break;
  default:
    break;
  }

  LED_POS = 48;
  switch (tens)
  {
  case 0:
    WriteZero();
    break;
  case 1:
    WriteOne();
    break;
  case 2:
    WriteTwo();
    break;
  case 3:
    WriteThree();
    break;
  case 4:
    WriteFour();
    break;
  case 5:
    WriteFive();
    break;
  case 6:
    WriteSix();
    break;
  case 7:
    WriteSeven();
    break;
  case 8:
    WriteEight();
    break;
  case 9:
    WriteNine();
    break;
  default:
    break;
  }
}

void serial_analysis(){
  Serial.println(counter);
  Serial.print(Ones);
  Serial.print("\t");
  Serial.println(tens);
}

void loop()
{
  if (counter == 100)
    counter = 0;
  else
    counter++;

  Ones = counter % 10;
  tens = counter / 10;
  writeNum();
  FastLED.show();
  delay(500);
}