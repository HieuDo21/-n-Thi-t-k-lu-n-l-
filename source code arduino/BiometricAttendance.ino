#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "fpc1020a.h"
#include "drawbit.h"
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's
#define Button 15
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "Phuong Quynh 1";
const char *password = "hoilamgi";
String postData ; 
String links = "http://192.168.0.101//test1/getdata.php"; 

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
/////////

HardwareSerial hwSerial(2);
FPC1020 Finger(&hwSerial);
extern unsigned char l_ucFPID;
extern unsigned char rBuf[192];
int8_t FingerID;
uint8_t id = 16;
void displaytext(String text, int x, int y);
void connectToWiFi();
void SendFingerprintID(int finger);
extern bool outMode2;
void IRAM_ATTR isr(){
  outMode2 = 0;
  //digitalWrite(2, !digitalRead(2));
}
void setup() {
  // put your setup code here, to run once:
  pinMode(Button, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(19, OUTPUT);
  attachInterrupt(Button, isr, FALLING);
  Serial.begin(19200);
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  //displaytext("Ready");
  display.clearDisplay();
  connectToWiFi();
}

void loop(){
  if(WiFi.status() != WL_CONNECTED){
    connectToWiFi();
  }
  digitalWrite(19,LOW);
    unsigned int User_ID = 0;
    unsigned char incomingNub;
    unsigned int  matchUserID = 0;
    unsigned char rtf = Finger.Search();
    switch (rtf){
      case TRUE:
      FingerID = int(l_ucFPID);
      break;
      case FALSE:
      FingerID = -1;
      //digitalWrite(4, HIGH);
      break;
      case ACK_TIMEOUT:
      FingerID = 0;
      break;
    }
    delay(50);
    DisplayFingerprintID();							
    //ChecktoAddID();
    check();
    ChecktoDeleteID();
    
}
void displaytext(String text, int x , int y){
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(x, y);
  display.println(text);
  display.display();
}
void printfinger(){
   display.clearDisplay();
    display.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, SH110X_WHITE);
      display.display();
}
//********************Get the Fingerprint ID******************
int getFingerprintID() {
  uint8_t p = Finger.Search();
  if (p){
    return int(l_ucFPID);
  }else{
    return -1;
  }  
}
///////////////////////////////////////////////////////////////
void SendFingerprintID( int finger ){

  WiFiClient client;
  HTTPClient http;  
  postData = "FingerID=" + String(finger); 
  http.begin(client,links); 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");   
  
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
  
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload
  Serial.println(postData);   //Post Data
  Serial.println(finger);     //Print fingerprint ID

  if (payload.substring(0, 5) == "login") {
    String user_name = payload.substring(5);
//  Serial.println(user_name);
    
    display.clearDisplay();
    display.setTextSize(2);             // Normal 2:2 pixel scale
    display.setTextColor(SH110X_WHITE);        // Draw SH110X_WHITE text
    display.setCursor(15,0);             // Start at top-left corner
    display.print(F("Welcome"));
    display.setCursor(0,20);
    display.print(user_name);
    display.display();
  }
  else if (payload.substring(0, 6) == "logout") {
    String user_name = payload.substring(6);
//  Serial.println(user_name);
    
    display.clearDisplay();
    display.setTextSize(2);             // Normal 2:2 pixel scale
    display.setTextColor(SH110X_WHITE);        // Draw SH110X_WHITE text
    display.setCursor(10,0);             // Start at top-left corner
    display.print(F("Good Bye"));
    display.setCursor(0,20);
    display.print(user_name);
    display.display();
  }
  delay(1000);
  
  postData = "";
  http.end();  //Close connection
}
///////////////////////////////////////



/********************connect to the WiFi******************/
void connectToWiFi(){
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SH110X_WHITE);        // Draw SH110X_WHITE text
    display.setCursor(0, 0);             // Start at top-left corner
    display.print(F("Connecting to \n"));
    display.setCursor(0, 50);   
    display.setTextSize(2);          
    display.print(ssid);
    display.drawBitmap( 73, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, SH110X_WHITE);
    display.display();
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
    
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SH110X_WHITE);        // Draw SH110X_WHITE text
    display.setCursor(8, 0);             // Start at top-left corner
    display.print(F("Connected \n"));
    display.drawBitmap( 33, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, SH110X_WHITE);
    display.display();
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP

}
////////////////////////////
void check(){
  
  //WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "Get_Fingerid=get_id"; // Add the Fingerprint ID to the Post array in order to send it
  // Post methode

  http.begin(links);  //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
  Serial.println(httpCode);
  if (payload.substring(0, 6) == "add-id") {
    String add_id = payload.substring(6);
    Serial.println(add_id);
    id = add_id.toInt();
    getFingerprintEnroll();
    
  }
  http.end();  //Close connection

  //digitalWrite(19, HIGH);
}


///////////////////////
//******************Enroll a Finpgerprint ID*****************

unsigned char getFingerprintEnroll() {

  unsigned char p = 0;
  while (p != TRUE){
    digitalWrite(4, !digitalRead(4));
    displaytext("Add Fingerprint,  please put your finger on the Fingerprint Sensor.", 14, 5);
    p = Finger.Enroll(id);
    switch (p){
      case TRUE:
        Serial.print("Success, your User ID is: "); 
        displaytext("Success, your User ID is: " + String(id),15,5);
        Serial.println(id);
        display.clearDisplay();
        display.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, SH110X_WHITE);
        display.display();
        break;
      case ACK_USER_OCCUPIED:
        Serial.println("Failed, this User ID alread exsits.");
        displaytext("Failed, this User ID alread exsits.",15, 5);
        break;
      case ACK_USER_EXIST:
        Serial.println("Failed, this fingerprint alread exsits.");
        displaytext("Failed, this fingerprint alread exsits.",15 ,5);
        break;
      case ACK_TIMEOUT:
        break;
      case FALSE:
        Serial.println("Failed, please try again.");
        displaytext("Failed, please try again.",15 ,5);
        break;
    }
    delay(500);
  }  
  confirmAdding();
  return p;
}
//******************Check if there a Fingerprint ID to add******************
void confirmAdding(){

  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "confirm_id=" + String(id); // Add the Fingerprint ID to the Post array in order to send it
  // Post methode

  http.begin(client,links); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  display.clearDisplay();
  display.setTextSize(1.5);             // Normal 1:1 pixel scale
  display.setTextColor(SH110X_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print(payload);
  display.display();
  delay(1000);
  Serial.println(payload);
  
  http.end();  //Close connection
}
void DisplayFingerprintID(){
  if (FingerID > 0){
    display.clearDisplay();
    display.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, SH110X_WHITE);
    display.display();
    delay(500);
    displaytext("Success, your User ID is: " + String(l_ucFPID),15, 5);
    delay(500);
    digitalWrite(19,HIGH);
    SendFingerprintID( FingerID );
  }
  else if (FingerID == 0){
    display.clearDisplay();
    display.drawBitmap( 32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, SH110X_WHITE);
    display.display();
  }
  else if (FingerID == -1){
    display.clearDisplay();
    display.drawBitmap( 34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, SH110X_WHITE);
    display.display();
  }
  //---------------------------------------------
  //Didn't find the scanner or there an error
  else if (FingerID == -2){
    display.clearDisplay();
    display.drawBitmap( 32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, SH110X_WHITE);
    display.display();
  }
}
void ChecktoDeleteID(){

WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "DeleteID=check"; // Add the Fingerprint ID to the Post array in order to send it
  // Post methode

  http.begin(client,links);  //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  if (payload.substring(0, 6) == "del-id") {
    String del_id = payload.substring(6);
    Serial.println(del_id);
    deleteFingerprint( del_id.toInt() );
  }
  
  http.end();  //Close connection
}
void  deleteFingerprint(int id){
    if(Finger.Delete(id)){
      display.clearDisplay();
      display.setTextSize(2);             // Normal 2:2 pixel scale
      display.setTextColor(SH110X_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.print(F("Deleted!\n"));
      display.display();
    }else{
      display.clearDisplay();
      display.setTextSize(2);             // Normal 2:2 pixel scale
      display.setTextColor(SH110X_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.print(F("Deleted FAILED!\n"));
      display.display();
    }
    delay(500);
}
