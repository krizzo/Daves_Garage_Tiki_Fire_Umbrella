# Episode 05 Refresh on rainbows and Marquee

## Table of Contents

- [Episode 05 Refresh on rainbows and Marquee](#episode-05-refresh-on-rainbows-and-marquee)
  - [Table of Contents](#table-of-contents)
  - [The wiring diagram](#the-wiring-diagram)
  - [Sections of code and video links](#sections-of-code-and-video-links)
    - [Starting Code (Refresher from Ep 4)](#starting-code-refresher-from-ep-4)
    - [Stage 2 Marquee](#stage-2-marquee)

This builds upon the previous episodes, watch Dave's video to get a better understanding.

## The wiring diagram

The wiring diagram for this code refer to E02 README.md for more details if this doesn't make any sense.

We're only adding 2 wires to the ESP32.

1) The Data IN cable for the LED strip to pin 23.
2) The EPS and LEDs need a common ground to make sure their clock signals are in phase. (I though I had faulty strips until I did all the same things on the arduino and identified this requirement).

```text
                                                              +-----------+
     +--------------------------------+                       | 5V Supply |
     |                                |                       +--+--+-----+
     | +-------------------------+    |                          |  |
     | |                         |    |                          |  |
+----+-+-----------------+       |    |      +-------------+     |  |      +--------------------------------------------+
|   21 22                |       |    |      |             |     |  +------+ VCC                                        |
|                        |       |    |      |   SSD1306   |     +---------+ GND         LED Strip of WS2812Bs          |
|      ESP 32 Board      |       |    |      |   Display   |     |  +------+ DIN                                        |
|                        |       |    |      |             |     |  |      +--------------------------------------------+
|   23      GND    3.3V  |       |    |      | SD SC VC GN |     |  |
+---+--------+------+----+       |    |      +--+-+--+--+--+     |  |
    |        |      |            |    |         | |  |  |        |  |
    |        |      |            |    +---------+ |  |  |        |  |
    |        |      |            |                |  |  |        |  |
    |        |      |            +----------------+  |  |        |  |
    |        |      |                                |  |        |  |
    |        |      +--------------------------------+  |        |  |
    |        |                                          |        |  |
    |        +------------------------------------------+--------+  |
    |                                                               |
    +---------------------------------------------------------------+
```

## Sections of code and video links

main.cpp will have the final code from the end of the video. Each section below will have the full code for that portion as a "check box" state to see what things are doing or as a reference for your own code you type up.

### Starting Code (Refresher from Ep 4)

```c++
#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define RED_PIN             16
#define GREEN_PIN           17
#define BLUE_PIN            18
#define STATUS_LED_PWM_FREQ 12000
#define STATUS_LED_PWM_RES  8

#define NUM_LEDS 56
#define LED_PIN  23

CRGB g_LEDs[NUM_LEDS] = {0};

#define OLED_DATA  21
#define OLED_CLOCK 22
#define OLED_RESET 255
U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_line_height = 0;

void write_led(int brightness = 0, bool invert = false)
{
  if(invert)
  {
    brightness = 256 - brightness;
  }
  ledcWrite(1, brightness);
  ledcWrite(2, brightness);
  ledcWrite(3, brightness);
}

double get_fps(double seconds)
{
  static double fps;
  fps = (fps * 0.9) + (1.0 / seconds * 0.1);
  return fps;
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  ledcAttachPin(RED_PIN,   1);
  ledcSetup(1, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(GREEN_PIN, 2);
  ledcSetup(2, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(BLUE_PIN,  3);
  ledcSetup(3, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);

  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");

  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_line_height = g_OLED.getFontAscent() - g_OLED.getFontDescent();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);
  FastLED.setBrightness(8);
}

void loop()
{
  bool bLED = 0;
  double fps = 0;

  uint8_t initHue = 0;
  const uint8_t deltaHue = 8;
  const uint8_t hueDensity = 4;

  for (;;)
  {
    bLED = !bLED;
    if (!bLED)
      write_led(0, true);
    else
      write_led(16, true);

    double dStart = millis() / 1000.0;

    g_OLED.clearBuffer();
    g_OLED.setCursor(0, g_line_height);
    g_OLED.printf("FPS: %.1lf", fps);
    g_OLED.sendBuffer();

    fill_rainbow(g_LEDs, NUM_LEDS, initHue += hueDensity, deltaHue);

    FastLED.show();

    double dEnd = millis() / 1000.0;
    fps = get_fps(dEnd - dStart);
  }
}
```

### Stage 2 Marquee

Starting to use header files to organize the code more. https://youtu.be/r6vMdnqUjTk?t=543

```c++
```
