
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

void DrawMarquee(void);

void DrawMarquee()
{
    static byte j = HUE_BLUE;
    j += 4;
    byte k = j;

    CRGB c;
    for (int i = 0; i < NUM_LEDS; i++)
        g_LEDs[i] = c.setHue(k+=8);

    static int scroll = 0;

    for (int i = scroll % 5; i < NUM_LEDS; i += 5)
        g_LEDs[i] = CRGB::Black;

    scroll++;

    delay(50);
}
