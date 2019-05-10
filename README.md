# USB Apple II Keyboard
Arduino Sketch to simulate a Apple II ASCII Keyboard

![Apple II USB Keyboard Interface](https://github.com/mafrmt00/Apple_II_Keyboard/blob/master/pictures/Keyb_Adapter.jpg "The assembled interface")

### Needed Hardware and Software

Hardware:
  - [Arduino Uno](https://en.wikipedia.org/wiki/Arduino_Uno)
  - [Arduino USB Host Shield](https://www.elecrow.com/wiki/index.php?title=USB_Host_Shield_for_Arduino
)
  - optional [Arduino Proto Board](https://store.arduino.cc/proto-shield-rev3-uno-size) with DIP Socket
  - Compatible USB keyboard. Tested with a Logitech K120.

Software:
  - Arduino IDE
  - USB Host Shield Library 2.0 (The modified Version included in this repo!)
  
### Installation
  - Create a folder "Apple_II_Keyboard" under your sketches dir and copy all files from the root dir of this repo there (The dir where the .ino lies). 
  - Copy the "USB_Host_Shield_Library_2.0" dir to you libraries dir.
  - Select Uno as board in the Arduino IDE
  - Use Upload in the Arduino IDE. If you can see some output in the serial monitor as you type everythin works.

### Connecting the Arduino to the Apple II
This Table shows how to connect the Apple II Keyboard Connector (A7) to the Arduino. The Pin definitions can also be changed in II_Encoder.h

| APPLE II DIP Pin| Function | Arduino Pin |
| --------------- | -------- | ----------- |
| 1               | +5V      | 5V          |
| 2               | Strobe   | 4           |
| 3               | Reset    | 5           |
| 4               | NC       | NC          |
| 5               | DATA 5   | 6           |
| 6               | DATA 4   | 7           |
| 7               | DATA 6   | 8           |
| 8               | GND      | GND         |
| 9               | NC       | NC          |
| 10              | DATA 2   | A0          |
| 11              | DATA 3   | A1          |
| 12              | DATA 0   | A2          |
| 13              | DATA 1   | A3          |
| 14              | NC       | NC          |
| 15              | -12V     | NC          |
| 16              | NC       | NC          |


### Setting the Arduino USB Host Shield for 5V operation

![Correctly jumpered Host Shield](https://github.com/mafrmt00/Apple_II_Keyboard/blob/master/pictures/USB_Host_Shield.jpg "Correctly jumpered Host Shield")
Make sure to set the solder jumpers as shoen in the picture. The Apple II and the UNO operate at 5V.

