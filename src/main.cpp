#include <Arduino.h>

#include "ssd1306Display.h"

// Setup SSD1306 screen
SsdDisplay display;

// Pins for the LED output
static const int8_t LED1 = 3;
static const int8_t LED_COUNT = 8;
static const int8_t LED_N = LED1 + LED_COUNT - 1; // Assume all LED pins are consecutive

static const int8_t PursuitButton = 13;
unsigned long currentTime = 0;
unsigned long lastTime = 0;
unsigned long buttonDelay_ms = 750;

// Flash modes
static const uint16_t regular_delay_ms = 200;
static const uint16_t pursuit_delay_ms = 50;
static const uint16_t delay_ms[] = {regular_delay_ms, pursuit_delay_ms};
static const int8_t modeCount = sizeof(delay_ms) / sizeof(delay_ms[0]);
int8_t flashMode = 0;

static const int8_t LED_ON = HIGH;
static const int8_t LED_OFF = LOW;

int8_t currentPin = LED1;
int8_t nextPin = LED1 + 1;
int8_t delta = 1;

void setup()
{

  // LED pin setup
  Serial.println(F("Initializing LEDs"));
  for (int8_t i = 0; i < LED_COUNT; ++i)
  {
    int8_t ledPin = LED1 + i;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LED_OFF);
  }

  // pursuit mode button
  pinMode(PursuitButton, INPUT);

  // Display intialization
  Serial.println(F("Initializing Dispalay"));
  if (display.init() != 0)
  {
    Serial.println(F("Initializing display failed"));
    // while (true)
    //   ;
  }
}

void loop()
{
  nextPin = currentPin + delta;
  if (nextPin < LED1 || LED_N < nextPin)
  {
    delta *= -1;
    nextPin = currentPin + delta;
  }

  digitalWrite(currentPin, LED_OFF);
  digitalWrite(nextPin, LED_ON);
  currentPin = nextPin;
  delay(delay_ms[flashMode]);

  if (digitalRead(PursuitButton))
  {
    currentTime = millis();
    if (currentTime - lastTime > buttonDelay_ms)
    {
      Serial.println(F("Flip"));
      lastTime = currentTime;
      flashMode = (flashMode + 1) % modeCount;
    }
  }
}
