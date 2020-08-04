#include <Arduino.h>
#include <U8g2lib.h>

#define RED_PIN             16
#define GREEN_PIN           17
#define BLUE_PIN            18
#define STATUS_LED_PWM_FREQ 12000
#define STATUS_LED_PWM_RES  8

#define OLED_DATA  21
#define OLED_CLOCK 22
#define OLED_RESET 255
U8G2_SSD1306_128X64_NONAME_F_SW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA);
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

void draw_display(int FPS)
{
  g_OLED.clearBuffer();
  g_OLED.home();

  g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());  g_OLED.setCursor(3, g_line_height * 2 + 2);
  g_OLED.print("Hello");
  g_OLED.setCursor(3, g_line_height * 3 + 2);
  g_OLED.print("World");
  g_OLED.setCursor(3, g_line_height * 4 + 2);
  g_OLED.printf("%03d", FPS);  for (int x = 0; x < g_OLED.getWidth(); x += 4)
    g_OLED.drawLine(x, 0, g_OLED.getWidth() - x, g_OLED.getHeight());  const int reticleY = g_OLED.getHeight() / 2;
  const int reticleR = g_OLED.getHeight() / 4 - 2;
  const int reticleX = g_OLED.getWidth() - reticleR - 8;

  for (int r = reticleR; r > 0; r -= 3)
    g_OLED.drawCircle(reticleX, reticleY, r);

  g_OLED.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10);
  g_OLED.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10);

  g_OLED.sendBuffer();
}

double get_fps(double seconds)
{
  static double fps;
  fps = (fps * 0.9) + (1.0 / seconds * 0.1);
  return fps;
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

  g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());
}

void loop()
{
  static bool bLED = LOW;
  int fps = 0;

  for (;;)
  {
    bLED = !bLED;
    if (!bLED)
      write_led(0, true);
    else
      write_led(16, true);

    double dStart = millis() / 1000.0;
    draw_display(fps);
    double dEnd = millis() / 1000.0;
    fps = get_fps(dEnd - dStart);
  }
}
