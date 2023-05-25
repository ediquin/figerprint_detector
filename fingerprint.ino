/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include "datos_oled.h"
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
//para el uso de websockets
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>


#include <Adafruit_GFX.h>          //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>      //https://github.com/adafruit/Adafruit_SSD1306
#include <Adafruit_Fingerprint.h>  //https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
const char *ssid = "Bruno";  //ENTER YOUR WIFI SETTINGS
const char *password = "DockyCatalina123.";
//************************************************************************
String postData ; // post array that will be send to the website
String link = "http://161.35.117.112/sitio_iea/datosESP.php"; //computer IP or the server domain
//String link = "http://192.168.100.8:84/ediquinweb-master/datosESP.php";
int FingerID = 0;     // The Fingerprint ID from the scanner 
uint16_t id;
void connectToWiFi();
#define Finger_Rx 14 //D5
#define Finger_Tx 12 //D6
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int buttonPin = 16;     // D0  cambio de modo
const int buttonPin2 = 10;    //lecura del sensor de huellas SD3
const int movim_1pin = 15;    //D8  temeperatura
const int movim_2pin = 13;    //D7  desinfección manos
const int analogPIN = A0;    //desinfección de cuerpo

int salida_manos = 0;    //D3
int salida_cuerpo = 2;    //D4
int buttonState = 0;    
int buttonState2 = 0;
int movim_1 = 0;
int movim_2 = 0;
int movim_3 = 0;
//temporizadores 
unsigned int t_huella = 2000;
unsigned int t_temperatura = 2000;
unsigned int t_desinfeccion = 3000;
unsigned int t_desinfec_cuerpo = 3000;
unsigned long int ahora = 0;
unsigned int tiempoAux = 0;
float temperatura = 0;

SocketIOclient socketIO;

SoftwareSerial mySerial(Finger_Rx, Finger_Tx);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define USE_SERIAL Serial

void setup()
{
  USE_SERIAL.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  USE_SERIAL.println("\n\nAdafruit finger detect test");
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(movim_1pin, INPUT);
  pinMode(movim_2pin, INPUT);
  pinMode(analogPIN, INPUT);
  pinMode(salida_manos, OUTPUT);
  pinMode(salida_cuerpo, OUTPUT);


  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    USE_SERIAL.println("Found fingerprint sensor!");
  } else {
    USE_SERIAL.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }


  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  connectToWiFi();
  mlx.begin();  
  //inicializar sockets
  socketIO.begin("161.35.117.112", 3000, "/socket.io/?EIO=4");
  // event handler
  socketIO.onEvent(socketIOEvent);
}

void loop()                     // run over and over again
{
  digitalWrite(salida_manos,LOW);
  digitalWrite(salida_cuerpo,LOW);
  socketIO.loop();
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    oled.clearDisplay();
    oled.drawBitmap( 0, 0, epd_bitmap_espera_128x64, 128, 64, WHITE);
    oled.display();
    ChecktoAddID();
    ChecktoDeleteID();
    delay(1000);
  } else {
      oled.clearDisplay();
      buttonState2 = digitalRead(buttonPin2);
      yield(); // para evitar bloqueos por WDT watchdog timer
      if(buttonState2 == LOW){
      FingerID = getFingerprintID();
      delay(300);
      DisplayFingerprintID();
      }
      FingerID = 0;
      DisplayFingerprintID();    
  }
  
}
int getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return 0;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return -2;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      return -2;
    default:
      //Serial.println("Unknown error");
      return -2;
  }
  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      return -2;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return -2;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return -2;
    default:
      //Serial.println("Unknown error");
      return -2;
  }
  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return -2;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Did not find a match");
    return -1;
  } else {
    //Serial.println("Unknown error");    
    return -2;
  }   
  // found a match!
  USE_SERIAL.print("Found ID #"); USE_SERIAL.print(finger.fingerID); 
  USE_SERIAL.print(" with confidence of "); USE_SERIAL.println(finger.confidence); 
  return finger.fingerID;
}
void Oled_mlx() {
oled.clearDisplay();
oled.setCursor(0,0); 
oled.setTextSize(1);
oled.setTextColor(WHITE);
oled.println("Temperatura en ºC:");
oled.setTextSize(2);
oled.setTextColor(WHITE);
oled.setCursor(10,20); 
oled.print("  ");
oled.println(mlx.readObjectTempC()+5);
temperatura = mlx.readObjectTempC()+ 5;
oled.display();
}

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    yield(); // para evitar bloqueos por WDT watchdog timer
    switch(type) {
        case sIOtype_DISCONNECT:
            USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

            // join default namespace (no auto join in Socket.IO V3)
            socketIO.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
            USE_SERIAL.printf("[IOc] get event: %s\n", payload);
            break;
        case sIOtype_ACK:
            USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            USE_SERIAL.printf("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
            
    }
}
