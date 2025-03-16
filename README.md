
## Overview

This project is an IoT application designed to run on an ESP32 microcontroller. It connects to a WiFi network, fetches data from a REST API, and displays the retrieved messages on an OLED screen.

---

### Functions

1. **`splashScreen()`**  
   - Displays a default startup screen on the OLED display.  
   - Shows a message icon and the text "Nokio" centered on the screen.

2. **`wifiSuccessScreen()`**  
   - Displays a screen indicating a successful WiFi connection.  
   - Shows a WiFi icon and the text "Wifi Connected" centered on the screen.

3. **`appGet(const char* _ENDPOINT, const char* _API_KEY)`**  
   - Sends an HTTP GET request to the specified endpoint with the provided API key.  
   - Returns the response body as a `String`. If the request fails, it returns the previous response body.

4. **`parseJSON(String _message, const char* _key)`**  
   - Parses a JSON string and extracts the value associated with the specified key.  
   - Returns the value as a `const char*`. If parsing fails, it returns the previous message.

5. **`printMessage(const char* _OLEDMessage1, const char* _OLEDMessage2)`**  
   - Displays two messages on the OLED screen.  
   - The messages are prefixed with "[msg]" and displayed in a formatted layout.

---

## Purpose

This code was written to fulfill the course requirements of **'ECSE3038 Engineering Internet of Things Systems'**. **Objective:** Apply IoT concepts using a physical ESP32 microcontroller to implement a Meassage App.

---

## Riddle

**"What is always on time but never arrives?"**

---
