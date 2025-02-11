# Complications connecting ESP32 board to Mac
It can be difficult to successfully connect a ESP32 development board to MacOS computer using USB. 
It turns out that the built in USB drivers on MacOS do not handle all of the chips used on ESP32 
boards to convert USB to serial UART that the ESP32 can handle. The ESP32 does not handle USB on its own.

The particular chip that had a problem with is the WCH CH340K. You can look at your 
ESP32 development for a small chip that is near the USB connector. It can be quite
difficult to read the chip info from the little chip. Easiest way to do so is to 
use your smartphone to take a really zoomed in picture of the chip. Below is a 
picture of the WCH CH340K chip used on the Elecrow ESP32 board:<br>
<img width="400" alt="CH340K problematic USB chip" src="https://github.com/user-attachments/assets/180517fb-315c-4e00-9a71-418828105a98" />

I encountered this problem with a Elecrow CrowPanel 4.3" TFT display ESP32-S3 board, though 
other boards 
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
Next step is to use the standard MacOS System Information tool.  In Launchpad go to the Other groud
and click on System Information icon. Then select USB on left side to see detailed info on USB connections. 
You will see multiple USB devices, including built in ones, but look for a USB Serial one similar to 
the one shown below. If you don't see such a device then it means that your USB cable is not connecting
to the ESP32 board at all.<br>
<img width="510" alt="sys_info" src="https://github.com/user-attachments/assets/399f4611-e430-444e-a6c2-5415ec68f1c9" />

Look specifically for the Product ID and the Vendor ID of the USB chip that used on your ESP32 board. 
A Product ID of 0x7522 and Vendor ID of 0x1a86 is for the CH340K chip from the vendor WCH. This is a 
chip that is used on the Elecrow 4.3" ESP32-S3 board but as of 2/10/25 is still not supported by MacOS. 
It could also be another Product ID from the company WCH that is not automatically supported. 
For unsupported USB chips you will need to load in a new driver onto your MacOS computer.

## Update drivers on MacOS to handle CH340K chip
`Note: these instructions are only for if the chip you are having trouble with is a WCH one, where 
System Information tool showed a Vendor ID of 0x1a86. `

The driver problem is rather complicated but the solution is quite simple. The reason it is 
complicated is because there is a lot of disinformation out there due to the drivers changing over 
the years. 

Key point is that while Apple does provide drivers for lots of USB chips, including those from WCH
like the CH340G, they don't supply drivers for all such chips. This means that most of the time 
the Apple drivers are sufficient. But the CH340K chip is newer and not supported. Therefore you need 
to install an updated driver. But to complicate things further, WCH has not provided a driver
that works with the CH340K on their GiitHub repo at https://github.com/WCHSoftGroup/ch34xser_macos . 

But someone let WCH know about the problem a few years ago and they addressed it and provided an updated
pkg file THAT WORKS! Simply go to https://github.com/skibu/modulencer/blob/main/misc/CH34xVCPDriver.pkg
, download the 
.pkg file onto your MacOS machine, and then execute the .pkg file to install the driver.
A dialog box should pop up stating there was a security issue and that CHW was trying to install a driver and needed permission. Go to System Settings and give permission to actually install the driver. Then system should state that your computer needs to be restarted.

Yes, this pkg file here is different from latest ones available from WCH GitHub repo. Was not able to get 
the one from the WCH GitHub repo to work.

In case you are interested how I found this magic driver see 
https://github.com/LilyGO/LILYGO-T-OI/issues/3#issuecomment-907645945  

## Verifying Driver
Once you have install the proper driver you can view driver extensions at /Library/Extensions/ . 
So make sure the Info.plist file contains the Product ID of the USB chip being used:
```
$ ls /Library/Extensions/CH34xVCPDriver.kext
Contents
$ ls /Library/Extensions/CH34xVCPDriver.kext/Contents/Info.plist
/Library/Extensions/CH34xVCPDriver.kext/Contents/Info.plist
```
The Info.plist file needs to contain the Product ID of the WCH chip that you are using. For my case 
it was the CH340K and the MacOS System Information tool showed a Product ID of 0x7522 in hex (29986). 
So search through your `/Library/Extensions/CH34xVCPDriver.kext/Contents/Info.plist` file and confirm
that it includes 29986 (or whatever your WCH chip Product ID is in decimal format) using:
```
$ grep 29986 /Library/Extensions/CH34xVCPDriver.kext/Contents/Info.plist
			<integer>29986</integer>
			<integer>29986</integer>
```
If 29986 is indeed in the file then take the next step.

## Reboot
To make sure the driver takes effect, reboot your computer.

## Verifying Communication
After the driver successfully loaded and you have rebooted your computer, hook up your ESP32
board to your MacOS computer via USB. Your computer should automatically create the needed /dev/tty 
file for communication:
```
$ ls /dev/tty.*
/dev/tty.BLTH				/dev/tty.wchusbserial14310
/dev/tty.Bluetooth-Incoming-Port
```
There should be a new "tty." (and "cu.") file called something like `/dev/tty.wchusbserial14310`

Configure VS Code to use the newly created device port.
Finally, try flashing the firmware again. It should now work!



