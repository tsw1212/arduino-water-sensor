# 💧 Smart Water and Heating Control System with ESP32

This project uses an ESP32 microcontroller to create a smart system that monitors water level and temperature, and controls a heating element and electric valve accordingly.

---

## 🚀 Main Components

- **ESP32 DevKit**
- **Ultrasonic Distance Sensor** – for measuring water level
- **DS18B20 Temperature Sensor** – for measuring water temperature
- **2 x Relays** – one for the heater, one for the electric valve
- **Libraries:**
  - `OneWire`
  - `DallasTemperature`
  - `Preferences` – to store user name in non-volatile memory

---

## ⚙️ Features

### Temperature Sensor (DS18B20)
- Measures water temperature in °C
- Invalid readings are detected (disconnected sensor or out-of-range values)

### Water Level Sensor (Ultrasonic)
- Measures distance from the sensor to the water surface
- If the distance is greater than the defined threshold → considered low water level

### Control Logic
- **Valve** turns on when the water level is low
- **Heater** turns on only when:
  - Water level is valid
  - Temperature is valid and below `40°C`

---

## 📝 Additional Features

- User name is stored in ESP32’s non-volatile memory usin
