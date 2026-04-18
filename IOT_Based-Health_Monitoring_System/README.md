# IOT based Health Monitoring System

## Overview
This project interfaces a **Pulse Sensor**, **DS18B20 Temperature Sensor**, **16x2_LCD-with_I2C_Module** with an **ESP8266 (NodeMCU)** and integrates it with **Arduino IoT Cloud**. The collected data is sent to the cloud for monitoring.

## Features
- Reads **heart rate** from the **Pulse Sensor**.
- Measures **temperature** using the **DS18B20 sensor**.
- Both values are displayed on 16x2_LCD. LCD shows no finger when finger is not on pulse sensor and shows pulse rate when finger is on the sensor
- Sends data to **Arduino IoT Cloud** for remote monitoring.
- Uses **WiFi** to establish a cloud connection.

## Components Required
- **ESP8266 NodeMCU**
- **Pulse Sensor**
- **DS18B20 Temperature Sensor**
- **16x2_LCD with I2C Module.**
- **10KΩ Resistor** (for DS18B20)
- **Jumper Wires**
- **Breadboard**

## Connections
| Component         | ESP8266 Pin |
|------------------|------------|
| Pulse Sensor     | A0         |
| DS18B20 Data Pin | D4         |
| DS18B20 VCC      | 3.3V       |
| DS18B20 GND      | GND        |
| LCD VCC          | VU (5V)    |
| LCD GND          | GND        |
| LCD SDA          | GPIO 4 (D2)|
| LCD SCL          | GPIO 5 (D1)|
| 4.7KΩ Resistor   | Between VCC & Data Pin|

## Setup Instructions
1. Connect the components as per the connection diagram.
2. Install the following Arduino libraries:
   - `OneWire`
   - `DallasTemperature`
   - `ArduinoIoTCloud`
   - `Arduino_ConnectionHandler`
   - `ESP8266WiFi`
   - `Wire`
   - `LiquidCrystal_I2C`
3. Configure your **WiFi SSID**, **Password**, and **Arduino IoT Cloud Credentials**.
4. Upload the code to your Node Mcu ESP8266.
5. Monitor sensor readings on the **16x2_LCD** or **Serial Monitor** or **Arduino IoT Cloud Dashboard**.

## Usage
Once uploaded, the ESP8266 will continuously read the **pulse rate** and **temperature** and send the data to **Arduino IoT Cloud**. The values can be viewed on the **16x2_LCD** & **cloud dashboard** in real-time.

## Creator
This project is designed by Prince Jha.

## License 
This project is open source and free for all :)
