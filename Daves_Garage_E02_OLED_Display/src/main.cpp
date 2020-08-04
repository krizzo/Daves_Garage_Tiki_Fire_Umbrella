#include <Arduino.h>
#include <U8g2lib.h>

#define RED_PIN             16
#define GREEN_PIN           17
#define BLUE_PIN            18
#define STATUS_LED_PWM_FREQ 12000
#define STATUS_LED_PWM_RES  8


#define OLED_DATA  21
#define OLED_CLOCK 22
U8G2_SSD1306_128X64_NONAME_F_SW_I2C g_OLED(U8G2_R0, OLED_CLOCK, OLED_DATA);
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

void setup()
{
  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  ledcAttachPin(RED_PIN,   1);
  ledcSetup(1, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(GREEN_PIN, 2);
  ledcSetup(2, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(BLUE_PIN,  3);
  ledcSetup(3, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);


  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_line_height = g_OLED.getFontAscent() - g_OLED.getFontDescent();

  g_OLED.setCursor(0, g_line_height);
  g_OLED.print("Hello, World!");
  g_OLED.setCursor(0, g_line_height * 2);
  g_OLED.printf("Line Height: %d", g_line_height);

  g_OLED.sendBuffer();
}

void loop()
{

  write_led(0, true);
  delay(500);
  write_led(16, true);
  delay(500);
}
