#include <Arduino.h>
#include <U8g2lib.h>

#define RED_PIN             16                                            // Onboard LED pins for status, carried over from Episode 1
#define GREEN_PIN           17
#define BLUE_PIN            18
#define STATUS_LED_PWM_FREQ 12000
#define STATUS_LED_PWM_RES  8

#define OLED_DATA  21                                                       // OLED display pins: carred over from Episode 2
#define OLED_CLOCK 22
#define OLED_RESET 255                                                      // This value is pulled from the default defined in the constructor.
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C g_OLED(U8G2_R2, OLED_CLOCK, OLED_DATA, OLED_RESET); // Stages 1 to 6: Set the OLED display object, stage 6 I got a rate of 9 fps
U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R0, OLED_RESET, OLED_CLOCK, OLED_DATA); // Stage 6.5: Optimization Set the OLED display object, jumpped to 27 fps
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

void draw_display(int FPS) // Stage 5 and 6: Draw function for FPS
{
  g_OLED.clearBuffer();
  g_OLED.home();

  g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());          // Draw a border around the display

  // Draw text in the frame on the left
  g_OLED.setCursor(3, g_line_height * 2 + 2);                             // Draw "Hello World" and framerate on the left hand side
  g_OLED.print("Hello");
  g_OLED.setCursor(3, g_line_height * 3 + 2);
  g_OLED.print("World");
  g_OLED.setCursor(3, g_line_height * 4 + 2);
  g_OLED.printf("%03d", FPS);                                             // Place Holder for the framerate

  // Draw a moire pattern like it's 1984
  for (int x = 0; x < g_OLED.getWidth(); x += 4)
    g_OLED.drawLine(x, 0, g_OLED.getWidth() - x, g_OLED.getHeight());

  // Draw a reticle using circles and lines
  const int reticleY = g_OLED.getHeight() / 2;                            // Vertical center of display
  const int reticleR = g_OLED.getHeight() / 4 - 2;                        // Slightly less than 1/4 screen height
  const int reticleX = g_OLED.getWidth() - reticleR - 8;                  // Right justified plus a margin

  for (int r = reticleR; r > 0; r -= 3)                                   // Draw series of nested circles
    g_OLED.drawCircle(reticleX, reticleY, r);

  g_OLED.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10); // H line through reticle
  g_OLED.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10); // V line through reticle

  g_OLED.sendBuffer();                                                    // Render the display

}


// Stage 6: Calculate the FPS
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
  pinMode(RED_PIN,   OUTPUT);                                             // Setup the status LED
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);
  ledcAttachPin(RED_PIN,   1);                                            // Set PWM gen for LED RED
  ledcSetup(1, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(GREEN_PIN, 2);                                            // Set PWM gen for LED GREEN
  ledcSetup(2, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);
  ledcAttachPin(BLUE_PIN,  3);                                            // Set PWM gen for LED BLUE
  ledcSetup(3, STATUS_LED_PWM_FREQ, STATUS_LED_PWM_RES);

  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
  g_line_height = g_OLED.getFontAscent() - g_OLED.getFontDescent();       // Descent is a negative number so we add it to the total

  // All Stages: Draw the border
  g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());          // Draw a border around the display

  // Stage 3: Draw text in the frame on the left
  // g_OLED.setCursor(3, g_line_height * 2 + 2);                             // Draw "Hello World" and framerate on the left hand side
  // g_OLED.print("Hello");
  // g_OLED.setCursor(3, g_line_height * 3 + 2);
  // g_OLED.print("World");
  // g_OLED.setCursor(3, g_line_height * 4 + 2);
  // g_OLED.printf("%03d", 0);                                               // Place Holder for the framerate

  // Stage 1: Draw a moire pattern like it's 1984
  // for (int x = 0; x < g_OLED.getWidth(); x += 4)
  //   g_OLED.drawLine(x, 0, g_OLED.getWidth() - x, g_OLED.getHeight());

  // Stage 2: Draw a reticle using circles and lines
  // const int reticleY = g_OLED.getHeight() / 2;                            // Vertical center of display
  // const int reticleR = g_OLED.getHeight() / 4 - 2;                        // Slightly less than 1/4 screen height
  // const int reticleX = g_OLED.getWidth() - reticleR - 8;                  // Right justified plus a margin

  // for (int r = reticleR; r > 0; r -= 3)                                   // Draw series of nested circles
  //   g_OLED.drawCircle(reticleX, reticleY, r);

  // g_OLED.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10); // H line through reticle
  // g_OLED.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10); // V line through reticle

  // Stages 1, 2, and 3
  // g_OLED.sendBuffer();                                                    // Render the display
}

void loop()
{
  // Stages 1, 2, and 3: Blink the onboard LED dimmed white
  // write_led(0, true);
  // delay(500);
  // write_led(16, true);
  // delay(500);

  // Stage 4: Moving things around make the code more organized
  // static bool bLED = LOW;
  // bLED = !bLED;

  // if (!bLED)                                                              // Slightly different as we have a dimming function.
  //   write_led(0, true);
  // else
  //   write_led(16, true);

  // g_OLED.drawFrame(0, 0, g_OLED.getWidth(), g_OLED.getHeight());          // Draw a border around the display

  // Draw text in the frame on the left
  // g_OLED.setCursor(3, g_line_height * 2 + 2);                             // Draw "Hello World" and framerate on the left hand side
  // g_OLED.print("Hello");
  // g_OLED.setCursor(3, g_line_height * 3 + 2);
  // g_OLED.print("World");
  // g_OLED.setCursor(3, g_line_height * 4 + 2);
  // g_OLED.printf("%03d", 0);                                               // Place Holder for the framerate

  // Draw a moire pattern like it's 1984
  // for (int x = 0; x < g_OLED.getWidth(); x += 4)
  //   g_OLED.drawLine(x, 0, g_OLED.getWidth() - x, g_OLED.getHeight());

  // Draw a reticle using circles and lines
  // const int reticleY = g_OLED.getHeight() / 2;                            // Vertical center of display
  // const int reticleR = g_OLED.getHeight() / 4 - 2;                        // Slightly less than 1/4 screen height
  // const int reticleX = g_OLED.getWidth() - reticleR - 8;                  // Right justified plus a margin

  // for (int r = reticleR; r > 0; r -= 3)                                   // Draw series of nested circles
  //   g_OLED.drawCircle(reticleX, reticleY, r);

  // g_OLED.drawHLine(reticleX - reticleR - 5, reticleY, 2 * reticleR + 10); // H line through reticle
  // g_OLED.drawVLine(reticleX, reticleY - reticleR - 5, 2 * reticleR + 10); // V line through reticle

  // g_OLED.sendBuffer();                                                    // Render the display

  // Stage 5 and 6: actual FPS metrics!
  static bool bLED = LOW;
  int fps = 0;

  for (;;)
  {
    bLED = !bLED;
    if (!bLED)                                                               // Slightly different as we have a dimming function.
      write_led(0, true);
    else
      write_led(16, true);

    // draw_display(fps); // Stage 5 only

    // Stage 6: Calculating the FPS
    double dStart = millis() / 1000.0;                                       // Record the start time
    draw_display(fps);
    double dEnd = millis() / 1000.0;                                         // Record the end time
    fps = get_fps(dEnd - dStart);                                            // Calculate the FPS rate
  }
}
