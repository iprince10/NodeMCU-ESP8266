# ESP8266 with 0.96Inch_OLED_SSD1306_Driver

This project demonstrates how to interface a 0.96 Inch OLED with an ESP8266 microcontroller. The OLED displays "Hii Prince" on first line and "Good day :)" on the second line.

## Designed By

**Prince Jha**

## Components Required

- ESP8266 (NodeMCU)
- 0.96 Inch OLED SSD1306 Driver
- Jumper wires
- Breadboard (optional)

## Circuit Diagram

| ESP8266 Pin  | LCD I2C Pin |
|--------------|-------------|
| 3.3 V           | VCC        |
| GND          | GND         |
| GPIO 4 (D2)  | SDA         |
| GPIO 5 (D1)  | SCL         |

Make sure to connect the OLED properly with the ESP8266 as shown above.

## How to Use

1. Connect the ESP8266 with the OLED as per the pinout provided.
2. Upload the code to the ESP8266 using the Arduino IDE.
3. Ensure you have installed the - `Adafruit SSD1306` , `Adafruit GFX` libraries.
4. Once uploaded, the OLED will display the following sequence:
   - "Hii Prince" (on the first line)
   - "Good day :)" (on the second line)

## Working Image

<table>
    <td align="left">
      <img src="https://raw.githubusercontent.com/iprince10/NodeMCU-ESP8266/main/0.96Inch_OLED_SSD1306/0.96Inch_OLED_ESP8266.jpeg" width="400">
    </td>
</table>

## License

This project is open-source. Feel free to modify and distribute it.
