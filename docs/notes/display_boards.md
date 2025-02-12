# Display Boards

A good number of displays with microcontrollers were examined. This document contains the 
rough notes for all of them.

Note: the term “HMI Display” means Human Machine Interaction Display. Basically, a touch screen. 

## General Info on Displays
As described at https://esp32io.com/tutorials/esp32-led-matrix there are 8x8 (4 for $10) https://www.amazon.com/dp/B0D2K9ZLW6 or  8x32 ($8 or 4 for $18) LED display https://www.amazon.com/dp/B0BXKKT72V
Could be really cool to have an 8x8 under transparent part of circuit board panel

13x9 RGB LED panel uses I2C $14.50 https://www.adafruit.com/product/5201 
Could be great for under front panel

Search for “Waveshare oled display” on aliexpress.com. Half price of Amazon
2.4” full color SPI for $11.39 + $4 shipping https://www.aliexpress.us/item/3256804999883432.html
1.5” for $12.01 + $4 shipping https://www.aliexpress.us/item/2251832806271164.html
Lots of smaller ones of course. 
1.28” ROUND 240x240 TFT SPI displays $3.30  https://www.aliexpress.us/item/3256805909598124.html for few dollars more can find touch sensitive ones which could make for a compelling UI

The OLED displays use SPI. Turns out can pretty easily control multiple displays by multiplexing via additional GPIO pins. Nice tutorial at https://randomnerdtutorials.com/esp32-spi-communication-arduino/
ESP32 has 2 SPI channels, but Daisy Seed only has 1.

Displaying animated GIFs https://youtu.be/omUWkUqFYrQ

PCBWAY.com has community portal where one can order the MJPEG Mini Video Player ESP32 video board described at https://youtu.be/67RFm2RMjC4
Display is 128x160

For displaying short video clips, like a fancy splash screen, on a larger 240x360 display could probably use uncompressed RGB565 format which is super simple because uncompressed. No processing power needed to uncompress! And would store video on SD card. Use ffmpeg to easily convert mp4 video to RGB565. https://github.com/moononournation/RGB565_video

Apparently resistive touch sensor not nearly as good as capacitive. So should look for display with capacitive touch screen. 
Example is 2.8” $26 https://www.waveshare.com/esp32-s3-touch-lcd-2.8.htm

## Makerfabs
Seems that want to use parallel instead of SPI communication with TFT display so that it can update fast enough. Plus SPI really tops out at 240x320. So probably good to get display with built in ESP32-S3 that uses parallel communication and has capacitive touch sensing. Has WiFi & Bluetooth. Uses ILI9488 driver chip.
3.5” 320x480 $44 - https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-ili9488.html
4” 480x480 $39 - https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-4-inch.html
See also details at  https://wiki.makerfabs.com/ESP32_S3_Parallel_3.5_TFT_with_Touch.html
[Useful details by reviewer](https://www.haraldkreuzer.net/en/news/first-impressions-new-makerfabs-matouch-esp32-s3-7-inch-ips-displays)

800x480 4.3” $39 5-point capacitive touch using GT911, with speaker https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-4-3-inch.html
800x480 4.3” $37 - https://www.tindie.com/products/makerfabs/esp32-s3-parallel-tft-with-touch-43/ 

## Elecrow.com
https://m.elecrow.com/pages/shop/shop?cid=104
480x320 3.5” https://m.elecrow.com/pages/shop/product/details?id=207932&=
Uses ILI9488 driver, but that’s unfortunately SPI.
Actually, also does parallel https://hackaday.io/project/3716-fpga-night-vision-goggles/log/12727-ili9341-fpga-parallel-driver-works
useful video https://youtu.be/HhNNu7Av7M4
ILI9341 is for 240x320, does parallel as well as SPI
=====> 4.3” 800x480 ESP32-S3 capacitive touch $30 https://m.elecrow.com/pages/shop/product/details?id=207883&= for $2.50 more get nice plastic case which could be great for a prototype. Has built in microphone and speaker. 
Wi-Fi & Bluetooth 5.0 (the fast kind needed for making music) with built in RF antenna
Has battery connector so would be easy to make it a standalone device. 
Has I2C so should be able to integrate in a F8R bank or the Expert Sleeper 8 socket CV input module. 
122mm x 75.6mm board (measured as 119mm x 71mm)
95.5mm x 54mm active screen or 109.7mm diag or 4.32”
Tutorial by Elecrow: https://youtu.be/W3LHGZRx7Jw
Many sizes available, uses GT911 touch chip, which should mean multi-touch, but says single-touch in https://m.elecrow.com/pages/shop/product/details?id=207883&=
￼
Lots of semi useful videos from the company https://youtube.com/@elecrow
Downsides:
* no ADC, DAC, nor GPIO available.  Though there are 9 gpio pins for the wireless module which won’t be using. 

Other Elecrow sizes:
* 3.5” 480x320 101.4mm x 63.3mm case   https://m.elecrow.com/pages/shop/product/details?id=207932&= 
* 4.3” 800 x 480 95.5mm x 54mm display, 122mm x 76mm circuit board (measured as 119mm x 71mm), 
* 5” 800 x 480 108mm x 65mm display, 132mm x 80.5mm circuit board,  $33 136mm x 85mm case https://m.elecrow.com/pages/shop/product/details?id=207872&=
* 7” 800x480 156mm x 87mm screen, 177mm x 105mm circuit board, 181mm x 108mm case, $35

Note: 115mm height available between rails. So even 7” screen has only 105mm (perhaps 100mm) high circuit board could still have room for a single horizontal row of knobs or sockets. 

![IMG_1474](https://github.com/user-attachments/assets/fecfeabf-c2dd-4754-bfbc-c90194e7a58d)

![121 9mm](https://github.com/user-attachments/assets/37da768a-5077-47e6-b73d-815f52f08c6e)

![IMG_1486](https://github.com/user-attachments/assets/d5a2185f-08e0-4118-99d7-9782f90564b1)

![IMG_1487](https://github.com/user-attachments/assets/853417e6-05c3-4650-b6bf-76b13e7b8c2f)

## Adafruit
They provide a 2.8” or 3.2” 240x320 TFT with breakout board - https://www.adafruit.com/product/1743
It does parallel bus, I2C capacitive touch, 3-5v
And of course there is documentation 
Good choice to start with, though makerfabs one has higher 320x480 res at 3.5” that has ESP32-S3 built in so needs no wiring

## Waveshare
480x800 4.3” ESP32-S3 https://www.waveshare.com/wiki/4.3inch_Capacitive_Touch_LCD 
$28 https://www.waveshare.com/4.3inch-capacitive-touch-lcd.htm
But apparently need Waveshare Open2491-C development board https://www.waveshare.com/wiki/Open429I-C which is $60 https://www.waveshare.com/Open429I-C-Standard.htm 
Actually, just need MCU STM32 Cortex-M4 MCU board which is $30 
Waveshare super hard to figure out and is more just for RasPi 

## Riverdi
Partnered with LGLV
$153 for 5” https://riverdi.com/product/5-inch-lcd-display-capacitive-touch-panel-optical-bonding-uxtouch-stm32u5-rvt50hqsnwc00-b

## Big Monitor 
Would be nice to be able to hook up big monitor. Best way is probably to use simple composite video output from the ESP32. Only crude grayscale, but there are libraries like https://youtu.be/QlGNAopvsOA . 
FAQ at https://youtu.be/wu5sySCW5NE
VGA board for a TInyESP32 at https://www.tindie.com/products/bitluni/pico-vga/ and https://www.tindie.com/products/bitluni/esp32-vga-black-edition/
Can get a composite to HDMI converter for $10  https://www.amazon.com/Converter-Amtake-Composite-Adapter-Supporting/dp/B07MJTR9KS 

GT911 capacitive touch chip
Looks quite good and very powerful 
I2C
Handles 5-point touch and gestures
Used by makerfab displays
Full documentation: https://orientdisplay.com/wp-content/uploads/2020/10/GT911-1.pdf
more info on touch displays: https://www.orientdisplay.com/knowledge-base/touch-panel-basics/
Resistive vs capacitance: https://www.orientdisplay.com/knowledge-base/touch-panel-basics/resistive-or-capacitive-touch-screen-which-is-better/. Capacitive supports multi-touch and gestures. 

## Display Sizes of various modules
Nerdseq:
has 3.5” display 1
Module is 158mm  wide
Panels are 5 1/8” 130mm high (spec is 5 1/4” 133mm)
Below panel module can be up to 4 1/2” 115mm high

Bitbox has 3” diag display. 
91mm wide

F8R is 82mm wide

Atom is 92mm wide

Befaco Oneiroi is 152mm wide!

Elecrow 4.3”:
122mm x 75.6mm board
95.5mm x 54mm active screen or 109.7mm diag or 4.32”

1hp is 0.2” or 5.08mm wide.
122mm 4.3” Elecrow is then 24hp, quite wide

Since only 115mm height available between rails and 4.3” display is 122mm tall it would have to be put in landscape mode (unless it could have tabs of panel be on top of rails, which it can’t since there are components on the underside all the way to the edges)

If in landscape mode then would still only have 39mm for sockets, knobs, and buttons. Could fit in just 3 rows horizontally of sockets. Could have 8-10 sockets per row. 
