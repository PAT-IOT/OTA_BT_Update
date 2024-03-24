# IoT Controller Firmware with Bluetooth Configuration and OTA Update
<p align="center">
  <img src="./image.png" alt="ESP32">
</p>
This firmware provides functionality for an IoT controller, including Bluetooth configuration and Over-The-Air (OTA) updates. It allows users to configure WiFi credentials via Bluetooth and update the firmware wirelessly.

## Features

- **Bluetooth Configuration**: Enables configuration of WiFi credentials via Bluetooth communication.
- **OTA Update**: Supports Over-The-Air updates for the firmware, allowing easy maintenance and upgrades.
- **Web Interface**: Provides a web interface for firmware updates using HTTP POST requests.
- **Authentication**: Implements basic authentication for accessing the web interface.

## Setup Instructions

To use this firmware for your IoT controller, follow these steps:

1. **Requirements**:
   - ESP32 board compatible with the Arduino framework.
   - Bluetooth module compatible with the ESP32.
   - WiFi network credentials.

2. **Installation**:
   - Clone or download this repository to your local machine.

3. **Configuration**:
   - Open the `PAT_OTA.h` file and modify the `ssid` and `password` variables to match your WiFi network credentials.
   - Optionally, adjust the `host` variable to set a custom hostname for the device.
   - Configure the Bluetooth device name by modifying the `device_name` variable in the `PAT_Bluetooth.h` file.

4. **Upload Firmware**:
   - Connect your ESP32 board to your computer.
   - Compile and upload the firmware to your ESP32 board.

5. **Bluetooth Configuration**:
   - Pair your Bluetooth-enabled device with the ESP32 using the device name specified in the configuration.
   - Use a terminal emulator or Bluetooth app to send commands for WiFi configuration:
     - To set the SSID, send `wlan0 <your_ssid>`.
     - To set the password, send `pass <your_password>`.
     - Restart the WiFi connection by sending `restart wlan0`.

6. **Web Interface for OTA Update**:
   - Once connected to the WiFi network, open a web browser and navigate to the device's IP address or hostname (e.g., `http://Novaday.local`).
   - Use the provided web interface to upload firmware files for OTA updates.

## Important Notes

- Ensure that your firmware files are compatible with the ESP32 platform and follow the required format.
- Make sure to secure your device by changing the default credentials and implementing additional security measures as needed.
- Test the firmware update process in a controlled environment before deploying it in production to ensure reliability.



