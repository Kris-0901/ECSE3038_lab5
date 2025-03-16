#include <Arduino.h>
//#include <string.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "env.h"

#define BUILTIN_LED 2
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // setup for OLED display

uint16_t messageIcon = 0x00EC; /*messageIcon (u8g2_font_streamline_all_t)*/
uint16_t wifiIcon = 0x01FD; /*wifiIcon (u8g2_font_streamline_all_t)*/

//function prototypes
void splashScreen(); //OLED default startup screen
void wifiSuccessScreen(); // indicate when connected to wifi
String appGet(const char* _ENDPOINT,const char* API_KEY);//get request body as char*
const char* parseJSON (String _message, const char* _key); //convert request body to JSON.Parse selected line then return message 
void printMessage(const char* _OLEDMessage); //print message to oled


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
    wifiSuccessScreen();
    delay(1000);
  }

  Serial.print("Conected to the WiFi Network with IP address: ");
  Serial.println(WiFi.localIP()); // print ip address 
  
}

void loop(){

  if ( WiFi.status() == WL_CONNECTED) // check if connected to Wifi
  {
    String response_body = appGet(ENDPOINT,API_KEY); //get request body as string 
    const char* message = parseJSON(response_body,"line_1");//convert request body to JSON.Parse line 1 then return message
    printMessage(message);
  }

}

String appGet(const char* _ENDPOINT,const char* _API_KEY="")
{
  HTTPClient https; // declare http object

  https.begin(_ENDPOINT); // connect to endpoint/url
  https.addHeader("api-key", _API_KEY);// add api-key to header of get request

  int _responseCode=https.GET();// get the RestAPI status code

  if (_responseCode<=0){// check for errors 
    const char* _ERROR = "ERROR";
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

const char* parseJSON (String _message, const char* _key)  //convert request body to JSON.Parse the specified key then return message as char*
{ 
  JsonDocument object; // initialize JSOn object 
  DeserializationError error = deserializeJson(object, _message); // check for json conversion error 

  if (error) // return errror if conversion fails 
  {
    const char* _ERROR = "ERROR";
    Serial.print("Deserialization failed: ");
    Serial.println(error.c_str());
    return _ERROR;
  }

  const char* _charMessage = object[_key];

  return _charMessage; 
}

void printMessage(const char* _OLEDMessage)
{
  const char* _title = "Messages"; ///screen title
  u8g2.firstPage();
  do {
    int _pos1 = 0;
    int _pos2 = 128;
    int _avaWidth = _pos2 - _pos1;
    u8g2.setFont(u8g2_font_6x13O_mf);// set font
    int _titleWidth = u8g2.getStrWidth(_title); // get width of string
    int _centerStart = _pos1 + (_avaWidth - _titleWidth) / 2; //print text in ceter of screen based on the width of text 
    u8g2.drawStr(_centerStart, 13, _title); 



    u8g2.setFont(u8g2_font_6x13O_mf);// set font
    int _charWidth = u8g2.getStrWidth("[msg]"); // get width of string header
    u8g2.drawStr(0, 30, "[msg]");
    u8g2.drawStr(_charWidth+2, 30, _OLEDMessage); // print message to oled in specified position 
  } while ( u8g2.nextPage() );
  return;
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
  return;
}

void wifiSuccessScreen()//OLED default startup screen
{
  const char* _title = "Wifi Connected";
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_streamline_all_t);// set font 
    u8g2.drawGlyph(53, 42, wifiIcon);// display an icon 21x21 pixels in center of screen 

    int _pos1 = 0;
    int _pos2 = 128;
    int _avaWidth = _pos2 - _pos1;
    u8g2.setFont(u8g2_font_t0_13b_me);// set font
    int _charWidth = u8g2.getStrWidth(_title); // get width of string
    int _centerStart = _pos1 + (_avaWidth - _charWidth) / 2; //print text in ceter of screen based on the width of text 
    u8g2.drawStr(_centerStart, 60, _title); 

  } while ( u8g2.nextPage() );
  return;
}