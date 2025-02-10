# Complications connecting ESP32 board to Mac
It can be difficult to successfully connect a ESP32 development board to MacOS computer using USB. 
It turns out that the built in USB drivers on MacOS do not handle all of the chips used on ESP32 
boards to convert USB to serial UART that the ESP32 can handle. The ESP32 does not handle USB on its own.

Encountered this problem with a Elecrow CrowPanel 4.3" TFT display ESP32-S3 board, though other boards 
can have the same issue. Also, this problem could happen with other operating systems as well.

## First, verify USB cable has communication lines
Many USB cables are only for power and do not have data communication lines, and therefore cannot be used
to communicate with the ESP32 board. 

The best way to determine if your USB cable has communication lines is to use a USB cable tester. 
These are pretty indexpensive and are inredibly useful if you use a lot of different USB cables.

Here is a problematic cable, as indicated by only power and ground LEDs lighting up:
<img width="400" alt="sys_info" src="https://github.com/user-attachments/assets/19840709-50ed-47f7-8c4e-93a57f886d73" />

And here is a good cable, as indicated by some of the communication LEDs lighting up:
<img width="400" alt="sys_info" src="https://github.com/user-attachments/assets/b7ebee78-1983-4d88-9b46-c17c84fc6ab5" />

## Verify that you are talking with the board
<img width="510" alt="sys_info" src="https://github.com/user-attachments/assets/399f4611-e430-444e-a6c2-5415ec68f1c9" />
