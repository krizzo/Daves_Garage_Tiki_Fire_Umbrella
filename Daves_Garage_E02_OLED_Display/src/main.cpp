#include <Arduino.h>
#include <U8g2lib.h>

#define RED_PIN             16                                      // State 1: Onboard LED pins for status, knowledge carried over from Episode 01
#define GREEN_PIN           17
#define BLUE_PIN            18
#define STATUS_LED_PWM_FREQ 12000
#define STATUS_LED_PWM_RES  8

// Stage 2 Writing to the OLED Display.
#define OLED_DATA  21                                                               // OLED display pins
#define OLED_CLOCK 22
U8G2_SSD1306_128X64_NONAME_F_SW_I2C g_OLED(U8G2_R2, OLED_CLOCK, OLED_DATA);         // Set the OLED display object
int g_line_height = 0;                                                              // Init the line hight for font calculation

void write_led(int brightness = 0, bool invert = false)             // State 1: Set an RGB LED to white and dimmed
{
  if(invert)                                                        // When we need to supply ground to the LED
  {
    brightness = 256 - brightness;
  }
  ledcWrite(1, brightness);
  ledcWrite(2, brightness);
  ledcWrite(3, brightness);
}

void setup()
{
  pinMode(RED_PIN,   OUTPUT);                                       // Stage 1: Setup the status LED
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  ledcAttachPin(RED_PIN,   1);                                      // Set PWM gen for LED RED
  ledcSetup(1, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(GREEN_PIN, 2);                                      // Set PWM gen for LED GREEN
  ledcSetup(2, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(BLUE_PIN,  3);                                      // Set PWM gen for LED BLUE
  ledcSetup(3, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);

  // Stage 2
  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_line_height = g_OLED.getFontAscent() - g_OLED.getFontDescent(); // Descent is a negative number so we add it to the total

  g_OLED.setCursor(0, g_line_height);                               // Place cursor at the bottom of the first line
  g_OLED.print("Hello, World!");
  g_OLED.setCursor(0, g_line_height * 2);
  g_OLED.printf("Line Height: %d", g_line_height);

  g_OLED.sendBuffer();                                              // Render the display
}

void loop()
{
  // Stage 1: Blink the onboard LED dimmed white
  write_led(0, true);
  delay(500);
  write_led(16, true);
  delay(500);
}
