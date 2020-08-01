# Episode 03 : OLED display graphics

This builds upon the previous episodes and doesn't go into detail on setting up the library. Watch Dave's video to get a better understanding.

This made the code a bit more messy with commenting out sections. I may break it out so each section below displays the code with the final code in `main.cpp`.

Stage 1: Draw lines, moire pattern  https://youtu.be/qmurt7uGH1Y?t=56

Stage 2: Draw a reticle using lines and circles. https://youtu.be/qmurt7uGH1Y?t=148

Stage 3: Draw some text and the place holder for the Frame Rate. https://youtu.be/qmurt7uGH1Y?t=411

Stage 4: Setup and loop code split. https://youtu.be/qmurt7uGH1Y?t=469

Stage 5: Functions and clean up time. https://youtu.be/qmurt7uGH1Y?t=583

Stage 6: Actual FPS metric calculation with optimization. https://youtu.be/qmurt7uGH1Y?t=778

The wiring diagram for this code refer to E02 README.md for more details if this doesn't make any sense.

```text
     +--------------------------------+
     |                                |
     | +-------------------------+    |
     | |                         |    |
+----+-+-----------------+       |    |      +-------------+
|   21 22                |       |    |      |             |
|                        |       |    |      |   SSD1306   |
|      ESP 32 Board      |       |    |      |   Display   |
|                        |       |    |      |             |
|           3.3V   GND   |       |    |      | SD SC GN VC |
+------------+------+----+       |    |      +--+-+--+--+--+
             |      |            |    |         | |  |  |
             |      |            |    +---------+ |  |  |
             |      |            |                |  |  |
             |      |            +----------------+  |  |
             |      |                                |  |
             |      +--------------------------------+  |
             |                                          |
             +------------------------------------------+

```
