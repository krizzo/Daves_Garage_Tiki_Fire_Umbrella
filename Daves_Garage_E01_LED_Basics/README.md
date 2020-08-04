# Episode 01 : LED basics

This code has 3 or more stages, it all depends how your mind works. I suggest watching the video from the beginning and trying to figure it out as Dave goes. I've included links below to the different sections of the video for quick reference.

I made comments in the code for each stage you just uncomment the code for the stage you're working on and comment all other stages, of course you can ignore the stage 3 function and global settings as they become noops.

## Stage 1

Basic Red blinking LED https://youtu.be/rlqbVttV0gI?t=379

The dimming Persistence of vision is how my original binary arduino clock worked. I didn't have enough power to light up all the LEDs nor the pin count so I used tricks like this.

```c++
#include <Arduino.h>

#define RED_PIN 16
#define GREEN_PIN 17
#define BLUE_PIN 18

void setup()
{
  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);

  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN,  HIGH);
}

void loop()
{
  // Dimming the LED by blinking faster, read up on persistence of vision
  digitalWrite(RED_PIN, LOW);
  delay(1);
  digitalWrite(RED_PIN, HIGH);
  delay(9);
}
```

## Stage 2

Using PWM (Pulse Width Modulation) to dim the LED https://youtu.be/rlqbVttV0gI?t=828

```c++
#include <Arduino.h>

#define RED_PIN 16
#define GREEN_PIN 17
#define BLUE_PIN 18

void setup()
{
  pinMode(RED_PIN,   OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN,  OUTPUT);

  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN,  HIGH);

  ledcAttachPin(RED_PIN, 1);        // Set PWM gen for LED RED
  ledcSetup(1, 12000, 8);
}

void loop()
{
  ledcWrite(1, 0);
  delay(100);
  ledcWrite(1, 256);
  delay(100);
}

```

## Stage 3

Rainbows! https://youtu.be/rlqbVttV0gI?t=966

main.cpp file
