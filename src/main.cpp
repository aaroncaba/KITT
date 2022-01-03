#include <Arduino.h>

#include "ssd1306Display.h"

// Setup SSD1306 screen
SsdDisplay display;

// Pins for the LED output
static const pin_size_t LED1 = 3;
static const pin_size_t LED_COUNT = 8;
static const pin_size_t LED_N = LED1 + LED_COUNT - 1; // Assume all LED pins are consecutive
pin_size_t currentPin = LED1 - 1;
pin_size_t nextPin = currentPin;
int8_t delta = 1; // must be signed so it can take on negative value

// button setup
static const pin_size_t DriveModeButton = 13;
unsigned long buttonDelay_ms = 750;

// Flash modes
struct FlashMode
{
  uint16_t delay_ms;
  String name;
};
static const uint16_t regular_delay_ms = 200;
static const uint16_t pursuit_delay_ms = 50;
static const uint16_t surveillance_delay_ms = 400;
static const FlashMode driveMode[] = {{regular_delay_ms, "Cruise"}, {pursuit_delay_ms, "Pursuit"}, {surveillance_delay_ms, "Surveillance"}};
static const int8_t modeCount = sizeof(driveMode) / sizeof(driveMode[0]);
int8_t flashMode = 0;

static const int8_t LED_ON = HIGH;
static const int8_t LED_OFF = LOW;

void printDriveMode()
{
  display.draw_text(2, 0, "Drive Mode:");
  display.draw_text(3, 0, "                  ");
  display.draw_text(3, 2, driveMode[flashMode].name);
}

void setup()
{
  // LED pin setup
  for (pin_size_t ledPin = LED1; ledPin <= LED_N; ++ledPin)
  {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LED_ON);
  }

  pinMode(DriveModeButton, INPUT);

  // Display intialization
  if (display.init() != 0)
  {
    while (true)
      ;
  }

  display.clearDisplay();
  display.draw_text(0, 0, "Press button");
  display.draw_text(1, 0, "to begin mission.");
  while (!digitalRead(DriveModeButton))
    ;

  display.clearDisplay();
  display.draw_text(0, 0, "Begin Mission!");
  delay(1000);
  printDriveMode();
}

void nextLED()
{
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastTime > driveMode[flashMode].delay_ms)
  {
    lastTime = currentTime;
    nextPin = currentPin + delta;
    if (nextPin < LED1 || LED_N < nextPin)
    {
      delta *= -1;
      nextPin = currentPin + delta;
    }

    digitalWrite(nextPin, LED_ON);
    delay(10);
    digitalWrite(currentPin, LED_OFF);
    currentPin = nextPin;
  }
}

void checkDriveModeButton()
{
  static unsigned long lastTime = 0;
  if (digitalRead(DriveModeButton))
  {
    unsigned long currentTime = millis();
    if (currentTime - lastTime > buttonDelay_ms)
    {
      lastTime = currentTime;
      flashMode = (flashMode + 1) % modeCount;
      printDriveMode();
    }
  }
}

void loop()
{
  nextLED();
  checkDriveModeButton();
}
