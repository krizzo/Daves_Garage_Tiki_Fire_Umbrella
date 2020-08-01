# Episode 02 : OLED display basics

This builds upon the first episode and doesn't go into detail on setting up the library. Watch Dave's video to get a better understanding. I had a breadboard and dupont wires already so I figured save me some time buying another ESP I just bought a cheap pack of SSD1306 displays and used the ezsbc from Episode 01.

Stage 1: Basic status LED blinking. https://youtu.be/W_gwDvxt7zc?t=535

Stage 2: Use U8G2 to write Hello World and then display the font height. https://youtu.be/W_gwDvxt7zc?t=622

The wiring diagram for this code is as follows if you didn't get an ESP32 with SSD1306 or other display soldered directly on the board.
You'll notice I don't have a RST and if you look at the global constructor under U8G2 it's not a requirement.

You just need a clock/data/ground/current pinout. I'm using a bread board I just "randomly" picked these two IO ports. You can easily get dupont female-female wires and connect the display if the chip and display already have male dupont headers soldered on.

You should refer to your chips data sheet for IO pins you can use

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
