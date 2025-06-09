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
## Implementation

### 1. Open the project in Arduino IDE

  - Launch Arduino IDE
  - Open **main.ino** file
  - Ensure the correct **Board** and **Port** are selected under Tools
### 2. Install the required libraries

  - There are two options:
  - Options 1:
    - Copy the contents of the **libraries/** folder into your Arduino libraries folder.
  - Option 2:
    - Go to **Sketch > Include Library > Manage Libraries**
    - Search and install the required libraries
   
### 3. Upload the sketch

  - Connect your board via USB
  - Click Verify and then the Upload button

## Configuration

You may need to edit the following parameters in **main.ino**:

  - #define WLAN_SSID "YOUR_WIFI_SSID"
  - #define WLAN_PASS "YOUR_WIFI_PASSWORD"

  - #define AIO_USERNAME  "YOUR_AIO_USERNAME"
  - #define AIO_KEY       "YOUR_AIO_KEY"
