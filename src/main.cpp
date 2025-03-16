#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "env.h"

#define BUILTIN_LED 2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // setup for OLED display

uint16_t messageIcon = 0x00EC; /*messageIcon (u8g2_font_streamline_all_t)*/

void splashScreen();

void setup(){

  Serial.begin(115200);

  u8g2.begin();  // Initialize the display
  u8g2.clear();  // clear the display
  splashScreen();

  if (IS_WOKWI) 
    WiFi.begin(SSID, PASS, CHANNEL);
  else 
    WiFi.begin(SSID, PASS);

  pinMode(BUILTIN_LED, OUTPUT);


  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  if ( WiFi.status() == WL_CONNECTED) {// turn on built in Led briefly when connected to wifi 
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1500);
    digitalWrite(BUILTIN_LED, LOW);
  }

  Serial.print("Conected to the WiFi Network with IP address: ");
  Serial.println(WiFi.localIP());


  
}

void loop(){
  if ( WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

  }
}

void splashScreen()
{
  const char* title = "Nokia";
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.drawGlyph(53, 42, messageIcon);

    int pos1 = 0;
    int pos2 = 128;
    int avaWidth = pos2 - pos1;
    u8g2.setFont(u8g2_font_luBIS14_te);
    int charWidth = u8g2.getStrWidth(title);
    int centerStart = pos1 + (avaWidth - charWidth) / 2;
    u8g2.drawStr(centerStart, 60, title);  //pass buffer // dummy data

  } while ( u8g2.nextPage() );

}