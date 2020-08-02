#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL // Stage 1: Get the FastLED library to use
#include <FastLED.h>

#define RED_PIN             16      // Onboard LED pins for status,
#define GREEN_PIN           17
#define BLUE_PIN            18
#define STATUS_LED_PWM_FREQ 12000
#define STATUS_LED_PWM_RES  8

#define NUM_LEDS 20                 // Stage 1:  FastLED definitions
#define LED_PIN  23                 // Data pin going to the LED strip

CRGB g_LEDs[NUM_LEDS] = {0};        // Stage 1: Frame buffer for FastLED

#define OLED_DATA  21               // OLED display pins
#define OLED_CLOCK 22
#define OLED_RESET 255                                                      // This value is pulled from the default defined in the constructor.
U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA); // Hardware offload of I2C Set the OLED display object, jumpped to 27 fps
int g_line_height = 0;                                                      // Init the line hight for font calculation

void write_led(int brightness = 0, bool invert = false)                   // Helper Function: Set an RGB LED to white and dimmed
{
  if(invert)                                                        // When we need to supply ground to the LED
  {
    brightness = 256 - brightness;
  }
  ledcWrite(1, brightness);
  ledcWrite(2, brightness);
  ledcWrite(3, brightness);
}

// Calculate the FPS
//
// Tracks a weighted average to smooth out the values that it calcs as the simple reciprocal
// of the amount of time taken specified by the caller.  So 1/3 of a second is 3 fps, and it
// will take up to 10 frames or so to stabilize on that value.
double get_fps(double seconds)
{
  static double fps;
  fps = (fps * 0.9) + (1.0 / seconds * 0.1);
  return fps;
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);       // Stage 1: Set our LED strip pin mode

  pinMode(RED_PIN,   OUTPUT);                                             // Setup the status LED
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  ledcAttachPin(RED_PIN,   1);                                            // Set PWM gen for LED RED
  ledcSetup(1, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(GREEN_PIN, 2);                                            // Set PWM gen for LED GREEN
  ledcSetup(2, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(BLUE_PIN,  3);                                            // Set PWM gen for LED BLUE
  ledcSetup(3, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);

  Serial.begin(115200);
  while (!Serial) { }
  Serial.println("ESP32 Startup");

  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_line_height = g_OLED.getFontAscent() - g_OLED.getFontDescent();       // Descent is a negative number so we add it to the total

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);       // Stage 1: Add our LED strip to the FastLED library
  FastLED.setBrightness(16);
}

void loop()
{
  bool bLED = 0;
  double fps = 0;

  // Stage 2
  uint8_t initHue = 0;
  const uint8_t deltaHue = 16;
  const uint8_t hueDensity = 4;

  for (;;)
  {
    bLED = !bLED;
    if (!bLED)                                                               // Slightly different as we have a dimming function.
      write_led(0, true);
    else
      write_led(16, true);

    double dStart = millis() / 1000.0;                                       // Record the start time

    // Draw the FPS on the OLED
    g_OLED.clearBuffer();
    g_OLED.setCursor(0, g_line_height);
    g_OLED.printf("FPS: %.1lf", fps);
    g_OLED.sendBuffer();

   // Stage 2: Handle LED strip
    fill_rainbow(g_LEDs, NUM_LEDS, initHue += hueDensity, deltaHue);

    FastLED.show();

    double dEnd = millis() / 1000.0;                                         // Record the end time
    fps = get_fps(dEnd - dStart);                                            // Calculate the FPS rate
  }
}
