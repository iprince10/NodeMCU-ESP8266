# Health Monitoring System — PCB Edition

An IoT-based health monitoring system built on a custom PCB with onboard power regulation, real-time body temperature and pulse sensing, and live data display via a 16×2 LCD with I2C module. Sensor data is pushed to Arduino IoT Cloud for remote monitoring, with live readings accessible from anywhere via the Arduino IoT Remote app on Android and iOS.

---

##  Project Preview

<table>
  <tr>
    <td align="left">
      <img src="https://github.com/iprince10/Health_Monitoring_System/blob/main/Health_Monitoring_System.jpeg" width="400">
    </td>
    <td align="right">
      <img src="https://github.com/iprince10/Health_Monitoring_System/blob/main/Health_Monitoring_System_Dashboard.png" width="400">
    </td>
  </tr>
</table>

*LCD displaying: `Pulse: No finger` / `Temp: 29.5 C`* <br>
*Dashboard displaying: `Pulse: 108` / `Temp: 24.0 C`*

---

## Related Repository

This project is an upgraded version of the breadboard prototype available here:  
[IOT Based Health Monitoring System (NodeMCU Breadboard Version)](https://github.com/iprince10/NodeMCU-ESP8266/tree/main/IOT_Based-Health_Monitoring_System)

### Key Differences from Breadboard Version

| Feature | Breadboard Version | PCB Version (This Repo) |
|---|---|---|
| Build | Breadboard | Custom PCB with soldered components using male-female burger strips |
| Power Input | USB 5V | 12V DC adapter |
| Voltage Regulation | External module | Onboard LM2596 DC buck converter (12V → 5V) |
| Peripheral Power | Mixed | All peripherals from onboard 3.3V rail |
| LCD Connection | Jumper wires | Directly connected via 5V onboard |
| Portability | Low | High — standalone unit |

---

## Features

- Real-time **body temperature** measurement using DS18B20 (1-Wire protocol)
- **Pulse/heart rate** monitoring using an analog pulse sensor (ADC)
- Live readings displayed on a **16×2 I2C LCD**
- **Wi-Fi enabled** via ESP8266 (NodeMCU)- supports Aruino IOT-cloud logging
- Arduino IoT Cloud integration - monitor temperature and pulse remotely in real time via the IoT Remote mobile app
- Onboard **LM2596 buck converter** for regulated 12V → 5V power conversion
- All sensor peripherals powered from the **onboard 3.3V rail**
- Compact, soldered PCB design - no breadboard or loose wires

---

## Hardware Components

| Component | Specification / Purpose |
|---|---|
| Microcontroller | ESP8266 NodeMCU (Wi-Fi enabled) |
| Temperature Sensor | DS18B20 (1-Wire, waterproof probe) |
| Pulse Sensor | Pulse sensor amped (ADC input) |
| Display | JHD 162A — 16×2 character LCD with I2C module |
| Power Supply | 12V DC wall adapter |
| Voltage Regulator | LM2596 DC-DC buck converter (12V → 5V) |
| PCB | Custom-designed, soldered |

---

## Power Architecture

```
12V DC Adapter
      │
   LM2596 Buck Converter
      │
     5V ──────────────────┬──── ESP8266 NodeMCU
                          ├──── 16×2 LCD
                          │
                   ESP8266 3.3V Onboard Regulator
                          │
                    ┌─────┴──────┐
                 DS18B20      Pulse Sensor
```

- **Input:** 12V DC from wall adapter
- **Regulated 5V:** Powers ESP8266 and LCD directly
- **Onboard 3.3V:** All sensor peripherals (DS18B20, pulse sensor) sourced from ESP8266's onboard 3.3V pin

---

## Pin Connections

| Peripheral | ESP8266 Pin |
|---|---|
| DS18B20 Data | D4 (GPIO2) |
| Pulse Sensor | A0 (ADC) |
| LCD SDA | D2 (GPIO4) |
| LCD SCL | D1 (GPIO5) |
| DS18B20 VCC | 3.3V |
| Pulse Sensor VCC | 3.3V |
| LCD VCC | 5V (onboard) |

---

## Software & Firmware

- **Firmware Language:** Arduino C++ (Arduino IDE)
- **Key Libraries:**
  - `OneWire` + `DallasTemperature` — DS18B20 communication
  - `LiquidCrystal_I2C` — LCD display
  - `ESP8266WiFi` — Wi-Fi connectivity
  - `Arduino IOT Cloud` — Cloud data logging
  - `Arduino_ConnectionHandler`

### LCD Output Format

```
Pulse: 72 BPM
Temp : 36.8 C
```

When no finger is detected on the pulse sensor:

```
Pulse: No finger
Temp : 29.5 C
```

---

## Getting Started

### Prerequisites

- Arduino IDE with ESP8266 board support installed
- Required libraries installed via Arduino Library Manager

### Steps

1. Clone this repository:
   ```bash
   git clone https://github.com/iprince10/Smart_Health_Monitoring_System.git
   ```
2. Open `Health_Monitoring_System.ino` in Arduino IDE
3. Set your Wi-Fi credentials and Arduino IOT Device ID and Secret key.
4. Select board: **NodeMCU 1.0 (ESP-12E Module)**
5. Upload to the ESP8266

---

## Cloud Integration

Sensor data can be pushed to **Arduino IoT Cloud** for remote monitoring and visualization.  
Configure your Arduino IOT Device ID and Secret key in the firmware before uploading. Monitor temperature and pulse remotely in real time via the IoT Remote mobile app

---

## Author

**Prince Jha**  
B.Tech, Electronics and Communication Engineering  
Jamia Hamdard (Deemed to be University), New Delhi  

[![GitHub](https://img.shields.io/badge/GitHub-iprince10-black?logo=github)](https://github.com/iprince10)

---

## License

This project is open-source and free for all :)
