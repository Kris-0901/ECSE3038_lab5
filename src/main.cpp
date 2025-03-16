#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "env.h"

#define BUILTIN_LED 2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // setup for OLED display

uint16_t messageIcon = 0x00EC; /*messageIcon (u8g2_font_streamline_all_t)*/

void splashScreen(); //OLED default startup screen function prototype

void setup(){

  Serial.begin(115200);

  u8g2.begin();  // Initialize the display
  u8g2.clear();  // clear the display
  splashScreen();//OLED default startup screen 

  if (IS_WOKWI) 
    WiFi.begin(SSID, PASS, CHANNEL);
  else 
    WiFi.begin(SSID, PASS);

  pinMode(BUILTIN_LED, OUTPUT);

  while (WiFi.status() != WL_CONNECTED) // wait for WiFi connection
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
  Serial.println(WiFi.localIP()); // print ip address 


  
}

void loop(){
  if ( WiFi.status() == WL_CONNECTED) {// check if connected to Wifi
    HTTPClient https; // declare http object

    https.begin(ENDPOINT); // connect to endpoint/url
    https.addHeader("api-key", API_KEY);// add api-key to header of get request

    int responseCode=https.GET();// get the RestAPI status code

    if (responseCode<=0){// check for errors 
      Serial.print("An error occured with repsonse code:");
      Serial.println(responseCode);
      https.end();
      return;
    }

    Serial.print("Status Code: ");// print stats code if no errors 
    Serial.println(responseCode);

    String response= https.getString(); // ge the body of the get request
    Serial.println(response); // print body of get requuest
    https.end();// end the https connection 

  }
}

void splashScreen()//OLED default startup screen
{
  const char* title = "Nokia";
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_streamline_all_t);// set font 
    u8g2.drawGlyph(53, 42, messageIcon);// display an icon 21x21 pixels in center of screen 

    int pos1 = 0;
    int pos2 = 128;
    int avaWidth = pos2 - pos1;
    u8g2.setFont(u8g2_font_luBIS14_te);// set font
    int charWidth = u8g2.getStrWidth(title); // get width of string
    int centerStart = pos1 + (avaWidth - charWidth) / 2; //print text in ceter of screen based on the width of text 
    u8g2.drawStr(centerStart, 60, title); 

  } while ( u8g2.nextPage() );

}