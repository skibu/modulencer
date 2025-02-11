# Display Hardware - Elecrow CrowPanel Advanced 7"

## Requirements
 * Largest display possible. 7". Originally thought that wanted to use 4.3" and possibly mount the module
   in portrait mode so that it is as narrow as possible. But found that even a 4.3" module would have to
   be used in landscape mode. Since have to use landscape mode might as well max out and use 7" display.
   Other option would be a 5" display. Large size makes module unique. Makes them more easy to use via touch screen.
   Plus recently larger displays have
   become cheaply available and they work well.
 * High-res - 800 x 480 pixels. If going to create a nice interface then need lots of resolution.
 * 16-bit 64k colors - Ideally would have 24-bit color like a regular computer but that takes twice as much
   memory as 16-bit color. And 16-bit color should still be great.
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
   

## EleCrow CrowPanel Advance 7.0" TFT display
The display that currently meets all the criteria is the CrowPanel Advance 7.0" TFT. Costs $34.90 (plus shipping).
https://www.elecrow.com/crowpanel-advance-7-0-hmi-esp32-ai-display-800x480-artificial-intelligent-ips-touch-screen-support-meshtastic-and-arduino-lvgl-micropython.html

There are also some bonus features of the CrowPanel display. It also has a built in microphone and can drive a 
speaker directly. 

![image](https://github.com/user-attachments/assets/830bb214-b0c7-422e-a169-f42a1ef8b4f5)

Note: first purchased a 4.3" CrowPanel since thought might want a smaller display. The display appears to be 
identical to the 7" version though so any software developed for the 4.3" display should automatically work
on the 7" display as well.
