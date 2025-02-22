# Display Hardware - Elecrow CrowPanel Advanced 7"

## Technology change
Key reason that pursuing creating a module using a large touch screen display for most of the UI is that in the last couple of years there have been a big change: the availability of a cheap, high-res, full color, responsive displays. This isn't about the display technology, but instead the availability of the ESP32-S3 microprocessor. 

#### ESP32-S3
The ESP32-S3 introduced significant new features:
 * Much more memory, which means that can handle larger display buffers. This allows one to use more bits per pixel, and thereby do full color. 
 * DMA - Direct Memory Access. Allows display buffers to be copied more quickly and without interfering with other tasks of the CPU. This also enables one to use more bits per pixel and thereby do full color.
 * lovyanGFX display driver - replacement for other GFX libs like Arduino GFX. lovyanGFX is much more optimized to run faster. In particular, it uses DMA when possible for the display buffers, making DMA actually usable.
 * Many vendors now provide 4.3" to 7" 800x480 pixel displays based on ESP32-S3. Therefore pricing is good. Higher resolution displays are a recent development since only with the ESP32-S3 can one handle the proportional large amount memory.

#### Processing power
Interestingly, CPU processing power is not really a factor since the clock rate of the S3 is the same as the previous generation S2. Therefore it is the other factors that are key. 

#### 16-bit parallel communication
Another factor is number of pins needed for CPU to communicate with the screen. For full color, SPI, the normally used serial communication scheme, is not sufficient. Need to transfer data in parallel. Therefore 16 bits should be used, which also means that using approximately 15 additional IO lines when compared to SPI. Note: 16 bits, or RGB 565, provides 65,536 colors, which is sufficient for full color. If would use 24 bits to get true full color then would actually need to use 32 bits per pixel of memory due to the way microprocessors work. Using twice as much memory is not worthwhile for a resolution in color that users will likely never notice.
5 bits (bits 15-11) for the Red value, 6 bits (10-6) for Green, and 5 bits (bits 4-0) for Blue level. Human eyes are more sensitive to green, hence an additional 16 levels. But the ESP32-S3 doesn't have more IO pins, which means that number of pins was not limiting previous displays. 

It should be noted though using additional 15 pins to achieve parallel communication leaves few IO pins for other purposes. Therefore it provides more reason to use two separate microprocessors, one for the display aand minimal IO, and the other for handling real-time music processing.

#### LVGL
Another factor is LVGL (Light and Versatile Graphics Library). It has been available for years now, but is recently even more important due to there now being more displays that can make use of it. 

#### EEZ Studio
GUI for creating UI via LVGL. https://www.envox.eu/ . Similar to SquareLine Studio, but is free. Was started 7 years go but seems to be now reaching significantly more usage.

Key features:
 * Drag-and-drop visual programming
 * Flowcharting
 * Support for LVGL (Light and Versatile Graphics Library)
 * Project templates and examples
 * Debugger
 * GUI simulator
 * Ability to create complex business logic
 * Modular architecture for future improvements and upgrades

## Requirements for Display
 * Largest display possible. 7". Originally thought that wanted to use 4.3" and possibly mount the module
   in portrait mode so that it is as narrow as possible. But found that even a 4.3" module would have to
   be used in landscape mode. Since have to use landscape mode might as well max out and use 7" display.
   Other option would be a 5" display. Large size makes module unique. Makes them more easy to use via touch screen.
   Plus recently larger displays have
   become cheaply available and they work well.
 * High-res - 800 x 480 pixels. If going to create a nice interface then need lots of resolution.
 * 16-bit 64k colors - Ideally would have 24-bit color like a regular computer but that takes twice as much
   memory as 16-bit color. And 16-bit color should still be great.
 * IPS Display - they look much better and brighter at an angled view.
 * 16-bit parallel display driver - Smaller displays typically use SPI display because of simplicity.
   But SPI is just one pixel at a time. That would be too slow for a high-res 800 x 480 display.
 * ESP32-S3 microprocessor - A high-res 800x480 display with 16 bit-color needs quite a bit of horse power and
   large frame buffers in order for display to be snappy. A standard ESP32 or even a ESP32-S2 is likely not
   enough oomph. Therefore definitely has to be a ESP32-S3 that comes with megs of RAM for framebuffer.
 * Inexpensive. The display is the most expensive component of the module. It needs to be reasonably priced
   in order for people to want to use it. Plus makes it easier to buy several and experiment with.
 * Must work with LVGL, EEZ Studio, PlatformIO, ESP-IDF, and VS Code - That is simply the most robust
   combination of tools.
 * SD card - so that can store large images, dynamic gifs, etc.
 * I2C accessible - Really might want to use I2C to communicate between the display ESP32 and the
   workhorse ESP32. Plus might want to connect I2C inputs, like a F8R faderbank directly to the display via
   I2C. With I2C one can communicate with multiple devices.
 * Must be readily available. For example, the Waveshare devices are out of stock, which is quite a nuisance.

Some useful info on display issues see https://www.haraldkreuzer.net/en/news/Using-Sunton-MaTouch-ESP32-S3-7-inch-displays-with-LVGL-and-ESP-IDF

## EleCrow CrowPanel Advance 7.0" 800x480 IPS display
The display that currently meets all the criteria is the CrowPanel Advance 7.0" TFT. Costs $34.90 (plus shipping).
https://www.elecrow.com/crowpanel-advance-7-0-hmi-esp32-ai-display-800x480-artificial-intelligent-ips-touch-screen-support-meshtastic-and-arduino-lvgl-micropython.html

There are also some bonus features of the CrowPanel display. It also has a built in microphone and can drive a 
speaker directly. 

![image](https://github.com/user-attachments/assets/830bb214-b0c7-422e-a169-f42a1ef8b4f5)

Note: first purchased a 4.3" CrowPanel since thought might want a smaller display. The display appears to be 
identical to the 7" version though so any software developed for the 4.3" display should automatically work
on the 7" display as well.

## Software and Tutorials
For the CrowPanel see the tutorial at https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File/tree/main/CrowPanel_ESP32_Tutorial

[LVGL lv_conf.h file](https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File/blob/main/CrowPanel_ESP32_Tutorial/File/LVGL%20lv_conf%20file/lv_conf.h)

[Tutorial Code](https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File/tree/main/CrowPanel_ESP32_Tutorial/Code) and here are useful looking [individual tutorials](https://github.com/Elecrow-RD/CrowPanel-ESP32-Display-Course-File/tree/main/CrowPanel_ESP32_Tutorial/Code/V1.X)

## Backup display
In case the CrowPanel is not available, here is a very similar display, though costs 30% more:

MaTouch ESP32-S3 Parallel TFT with Touch 7“
$48.90 1024x600 (the 800x480 is an inerior non-IPS display)
https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-7-inch.html
[Useful details](https://www.haraldkreuzer.net/en/news/first-impressions-new-makerfabs-matouch-esp32-s3-7-inch-ips-displays)

## Info on all other possible displays
See https://github.com/skibu/modulencer/blob/main/docs/notes/display_boards.md
