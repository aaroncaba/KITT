#include "ssd1306Display.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

int SsdDisplay::init()
{
    // Initialize the display
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDR))
    {
        Serial.println(F("SSD1306 allocation failed"));
        return -1;
    }

    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE, BLACK);

    return 0;
}

// Draw text at (x,y) pixel location
void SsdDisplay::draw_text(const int16_t &x_pos, const int16_t &y_pos, const char *text, const byte &text_size)
{
    display.setCursor(x_pos, y_pos);
    display.setTextSize(text_size);
    display.print(text);
    display.display();
}

// Draw text at (row,column) character location
void SsdDisplay::draw_text(const int16_t &row, const int16_t &col, const String &text)
{
    static const int16_t char_width = 6;
    static const int16_t char_height = 10;
    draw_text(col * char_width, row * char_height, text.c_str(), 1);
}

void SsdDisplay::clearDisplay()
{
    display.clearDisplay();
}