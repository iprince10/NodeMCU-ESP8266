# ESP8266 with 16x2 LCD (I2C) Interface

This project demonstrates how to interface a 16x2 LCD using the I2C module with an ESP8266 microcontroller. The LCD displays "Pulse Rate :" on first line and "Temparature: " on the second line.

## Designed By

**Prince Jha**

## Components Required

- ESP8266 (NodeMCU)
- 16x2 LCD with I2C module
- Jumper wires
- Breadboard (optional)

## Circuit Diagram

| ESP8266 Pin  | LCD I2C Pin |
|--------------|-------------|
| VU           | VCC        |
| GND          | GND         |
| GPIO 4 (D2)  | SDA         |
| GPIO 5 (D1)  | SCL         |

Make sure to connect the I2C module properly with the ESP8266 as shown above.

## How to Use

1. Connect the ESP8266 with the LCD using the I2C module as per the pinout provided.
2. Upload the code to the ESP8266 using the Arduino IDE.
3. Ensure you have installed the - `LiquidCrystal_I2C.h` library:
4. Once uploaded, the LCD will display the following sequence:
   - "Pulse Rate :" (on the first line)
   - "Temparature :" (on the second line)

## License

This project is open-source. Feel free to modify and distribute it.
