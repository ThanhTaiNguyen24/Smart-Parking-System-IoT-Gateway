# Smart-Parking-System-IoT-Gateway with Arduino IDE

This project implements an IoT Gateway using an Micro:bit, built and uploaded via the **Arduino IDE**. The gateway collects data from connected sensors and forwards it to the **Adafruit IO** server via MQTT. Additionally, the gateway listens for incoming control messages or configurations from Adafruit IO to respond or adjust its behavior accordingly.
## Requirements

- Arduino IDE
- Board: Micro:bit
- Installed libraries:
  - List of external libraries used (can be in `libraries/` folder)
  - For example:
    - `Adafruit_LiquidCrystal.h`
    - `Adafruit_MQTT_Library.h`
    - `DHT20.h`
    - `Adafruit_NeoPixel.h`
