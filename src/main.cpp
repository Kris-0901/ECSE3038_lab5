#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "env.h"

#define BUILTIN_LED 2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // setup for OLED display

uint16_t messageIcon = 0x00EC; /*messageIcon (u8g2_font_streamline_all_t)*/

String app_get(const char* _ENDPOINT,const char* API_KEY);
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

  if ( WiFi.status() == WL_CONNECTED) 
  {// check if connected to Wifi
    String response_body = app_get(ENDPOINT,API_KEY); //get request body as string 
  }

}

String app_get(const char* _ENDPOINT,const char* _API_KEY="")
{
  HTTPClient https; // declare http object

  https.begin(_ENDPOINT); // connect to endpoint/url
  https.addHeader("api-key", _API_KEY);// add api-key to header of get request

  int _responseCode=https.GET();// get the RestAPI status code

  if (_responseCode<=0){// check for errors 
    String _ERROR = "ERROR";
    Serial.print("An error occured with repsonse code:");
    Serial.println(_responseCode);
    https.end();
    return _ERROR;
  }

  Serial.print("Status Code: ");// print stats code if no errors 
  Serial.println(_responseCode);

  String _response_body= https.getString(); // ge the body of the get request
  Serial.println(_response_body); // print body of requuest
  https.end();// end the https connection 

  return _response_body;
}

void splashScreen()//OLED default startup screen
{
  const char* _title = "Nokia";
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_streamline_all_t);// set font 
    u8g2.drawGlyph(53, 42, messageIcon);// display an icon 21x21 pixels in center of screen 

    int _pos1 = 0;
    int _pos2 = 128;
    int _avaWidth = _pos2 - _pos1;
    u8g2.setFont(u8g2_font_luBIS14_te);// set font
    int _charWidth = u8g2.getStrWidth(_title); // get width of string
    int _centerStart = _pos1 + (_avaWidth - _charWidth) / 2; //print text in ceter of screen based on the width of text 
    u8g2.drawStr(_centerStart, 60, _title); 

  } while ( u8g2.nextPage() );

}