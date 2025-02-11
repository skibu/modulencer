# Miscellaneous Hardware Notes

## Signal Amplifier
The Daisy Seed and everything else will put out low voltage audio out. Need to increase amplitude to get it to eurorack levels
TL07x data sheet https://www.ti.com/lit/ds/symlink/tl071.pdf
TL071x 5-pin package has one op-amp, but can have 2 offset inputs
TL072x 8-pin package has two op-amps
TL074x 14-pin package has four op-amps
So last digit just appears to indicate number of op-amps on device
Appears the H devices are the next generation and better for filtering RF noise
CGS circuit by Ken Stone is highly recommended - https://github.com/mamonulabs/CGS_Synth/blob/master/cgs60_sba.pdf   https://web.archive.org/web/20170317221750/https://cgs.synth.net/modules/cgs60_sba.html
￼
![To Syrth](https://github.com/user-attachments/assets/02fc7ac5-be3f-4be5-b904-27b45b6cf740)
![IMG_1472](https://github.com/user-attachments/assets/898ca0e9-287a-4b50-b259-bccc65f20a40)
￼
Note that for the TL071 top 2 pins and lower right one don’t appear to be connected.  This corresponds to the TL071H data sheet shown below, and means that the offset inputs of the TL071 indeed not used, so can use a TL072H 8-pin package and really save on space and components. 
![Not to scale](https://github.com/user-attachments/assets/d3f142e9-12b9-4fe0-8fb1-fe1c92c9fbbd)
￼
Since there are lots of fake parts out there best to go with company like Mouser.com where TL072CP are 10 for $5.10

Another example of amp (3.3v -> 10v) is in https://youtu.be/fud-q7li9Hw

MIDI 
Super simple midi input and output circuits https://youtu.be/0L7WAMFWSgY?t=129
No op-amps or capacitors! Uses 6N138 opto coupler for input
Same info at https://youtu.be/l34CNfwfuIY?t=313

SD Card
Can use SPI to talk with SD card module https://embetronicx.com/tutorials/microcontrollers/stm32/stm32-sd-card-interfacing-with-example/
Can also use SDIO https://controllerstech.com/interface-sd-card-with-sdio-in-stm32/
SD card module $3.50 handles both SPI and SDIO https://www.adafruit.com/product/4682
I think SDIO is faster plus can probably just use a socket instead of a whole module. 

CV Output
Useful https://youtu.be/mrvgZEVAcTk
Might want an 12-bit DAC with SPI, but also need voltages to be 0 - 10v instead of 0-3.3V.
Quad 12-bit DAC with I2C 0-5V $7.50  https://www.adafruit.com/product/4470

MCP4728 seems like popular quad DAC chip for under $2 https://components101.com/ics/mcp4728-12-bit-quad-digital-to-analog-converter
More from microchip https://www.microchip.com/en-us/products/data-converters/digital-to-analog-converters
I2C octal 12-bit $9 https://www.microchip.com/en-us/product/MCP47CMB28
And SPI https://www.microchip.com/en-us/product/MCP48CMB28

Sliders
https://store.spectrasymbol.com/

LED ring
1” diameter 20 RGB LEDs $11.50 https://www.sparkfun.com/sparkfun-lumini-led-ring-1-inch-20-x-apa102-2020.html

Can get knockoff rings from DIYmall, 5 for $19 https://www.amazon.com/DIYmall-WS2812B-Integrated-Individually-Addressable/dp/B0B2D5QXG5/ref=sr_1_15

MIDI controller
Was originally thinking that wanted a few velocity sensitive buttons on the module so that could input a track manually by finger drumming. But space is limited. And would be hard to provide enough inputs. Solution is to just use a commercial MIDI controller like the $99 Akai MPK https://www.amazon.com/AKAI-PROFESSIONAL-MPK-Mini-MK3/dp/B0886KWVYR/ . Not only has 8 velocity sensitive pads,  it a full mini keyboard, 8 knobs, and a joystick. 

$76 for used Akai https://reverb.com/item/86393906-used-akai-mpk-mini-mk3

Argh, except the Akai MPK is a USB midi keyboard and needs to plug into a USB host, like a computer. There are midi usb host boxes available but they are an additional $50.

Cheapest Midi host device is $43 https://www.amazon.com/DigitalLife-Stand-Alone-MIDI-Host-Bridge/dp/B0DM2132KV/

A real Doremidi one for $35.55 https://www.aliexpress.us/item/3256802637245554.html

Might need MAX3421E chip which is $5.40 https://www.analog.com/en/products/max3421e.html . There is a $15 Adafruit board https://www.adafruit.com/product/5858

Can use a CH559 chip https://m.youtube.com/watch?v=po3FBdY0GS4
$1.63 https://www.lcsc.com/product-detail/WCH_WCH-Jiangsu-Qin-Heng-CH559L_C150548.html
Software to run on  the CH559 https://github.com/atc1441/CH559sdccUSBHost

Can buy a little board with a programmed CH559 for $9.50 https://www.tindie.com/products/matzelectronics/ch559-usb-host-to-uart-bridge-module/

Apparently a Teensy 4.0 can be a usb host https://github.com/PaulStoffregen/USBHost_t36

Perhaps an esp32 software solution https://github.com/tobozo/ESP32-USB-Soft-Host and https://github.com/sdima1357/esp32_usb_soft_host

Official documentation from Espressif on using ESP32-S2/S3 as USB host, but nothing specifically about MIDI https://docs.espressif.com/projects/esp-iot-solution/en/latest/usb/usb_overview/usb_host_solutions.html

Documentation for Espressif on TinyUSB, a library by Adafruit, that runs on ESP32-S2/S3 and explicitly handles MIDI https://docs.espressif.com/projects/esp-iot-solution/en/latest/usb/usb_overview/tinyusb_guide.html

TinyUSB host example https://github.com/rppicomidi/midi2usbhost

Or maybe here https://github.com/rppicomidi/usb_midi_host

And can just look at https://docs.tinyusb.org/en/latest/ or TinyUSB.org 

Info on esp32 being a midi device (not a host unfortunately) https://github.com/espressif/esp-idf/tree/master/examples/peripherals/usb/device/tusb_midi

Seems that tinyUSB should already work, maybe https://github.com/adafruit/Adafruit_TinyUSB_Arduino/issues/360

Misc Parts
Digital rotary encoder 5 for $10 https://www.amazon.com/dp/B07F24TRYG/

Thonk is great source of DIY parts https://www.thonk.co.uk/product-category/parts/

Befaco Jack nuts look quite nice https://www.thonk.co.uk/shop/bananuts/
