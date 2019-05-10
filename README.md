# Apple_II_Keyboard
Arduino Sketch to simulate a Apple II ASCII Keyboard

Hardware:
  - [Arduino Uno](https://en.wikipedia.org/wiki/Arduino_Uno)
  - [Arduino USB Host Shield](https://www.elecrow.com/wiki/index.php?title=USB_Host_Shield_for_Arduino
)
  - optional [Arduino Proto Board](https://store.arduino.cc/proto-shield-rev3-uno-size) with DIP Socket

Tghis Table shows how to connect the Apple II Keyboard Connector (A7) to the Arduino. The Pin definitions can also be changed in II_Encoder.h

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
