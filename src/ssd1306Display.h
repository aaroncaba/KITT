#ifndef SSD1306DISPLAY_H_
#define SSD1306DISPLAY_H_

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define SSD1306_ADDR 0x3C // SSD1306 display I2C address

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)

class SsdDisplay
{
public:
    SsdDisplay(){};
    int init();
    void draw_text(const int16_t &x_pos, const int16_t &y_pos, const char *text, const byte &text_size);
    void draw_text(const int16_t &row, const int16_t &col, const String &text);
    void clearDisplay();

private:
    Adafruit_SSD1306 display;
};

#endif