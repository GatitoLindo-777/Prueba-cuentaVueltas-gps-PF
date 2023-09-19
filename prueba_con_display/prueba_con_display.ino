#include <TinyGPSPlus.h>
#include <SPI.h>    // incluye libreria bus SPI
#include <Adafruit_GFX.h> // incluye libreria para manejo de graficos
#include <Adafruit_ILI9341.h> // incluye libreria para controlador ILI9341
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include <MPU6050.h>
#include <I2Cdev.h>

//#include <SoftwareSerial.h>

#define PIN_BTN 15
#define VUmbral 0.00001
#define TFT_DC 4
#define TFT_CS 15
#define TFT_RST 2
#define TFT_MISO 19         
#define TFT_MOSI 23           
#define TFT_CLK 18
#define RXD2 16
#define TXD2 17

void displayInfo(void);

double posLat;
double posLng;

float posLatUnsigned;
float posLngUnsigned;

double UmbralLatM; //lat+
double UmbralLatm; //Lat-
double UmbralLngM; //Lng+
double UmbralLngm; //Lng-

float Vpor = 0.00001;

int contador;
#define boton 0
#define comprobacion 1
#define lectura 2
int vueltas;

/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
//SoftwareSerial ss(RXPin, TXPin);

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup()
{
  tft.begin(); // inicializa pantalla
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXPin, TXPin);
  Serial2.begin(GPSBaud);
  pinMode (PIN_BTN, INPUT_PULLUP);

  tft.setRotation(3);       // establece posicion vertical con pines hacia abajo
  tft.fillScreen(ILI9341_BLACK);    // fondo de pantalla de color negro
  tft.fillRect(0, 0, 320, 30, ILI9341_NAVY);  // rectangulo azul naval a modo de fondo de titulo 
  tft.setTextColor(ILI9341_WHITE);    // color de texto en blanco
  tft.setTextSize(2);       // escala de texto en 2
  tft.setCursor(82,10);        // ubica cursor
  tft.print("TRACK X TRACK");    // imprime texto

  /*Serial.println(F("DeviceExample.ino"));
    Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
    Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();*/
  /*while (gps.satellites.value() < 5) {
    Serial.println("hay menos de 5 satelites conectados");
    Serial.print("satelites actuales ");
    Serial.println(gps.satellites.value());
    delay(1000);
  }*/
  
}
void loop() {

  if (Serial.read() == '+') {
    Vpor += 0.00001;
    Serial.print(Vpor);
  } else if (Serial.read() == '-') {
    Vpor -= 0.00001;
    Serial.print(Vpor);
  }
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      //displayInfo();
    }
  }

  switch (contador) {
    case boton:
      if (digitalRead(PIN_BTN) == 0) {
        posLat = gps.location.lat();
        posLng = gps.location.lng();
        Serial.print("posLat");
        Serial.println(posLat, 6);
        Serial.print("posLng");
        Serial.println(posLng, 6);

        float  variacionLat;
        float variacionLng;
        if (posLat < 0) {
          posLatUnsigned = posLat * (-1);
        } else {
          posLatUnsigned = posLat;
        }
        if (posLng < 0) {
          posLngUnsigned = posLng * (-1);
        } else {
          posLngUnsigned = posLng;
        }

        variacionLat = posLatUnsigned / 100 * Vpor;
        variacionLng = posLngUnsigned / 100 * Vpor;

        UmbralLatM = posLat + variacionLat;
        UmbralLatm = posLat - variacionLat;
        UmbralLngM = posLng + variacionLng;
        UmbralLngm = posLat - variacionLng;

        Serial.println(UmbralLatM);
        Serial.println(UmbralLatm);
        Serial.println(UmbralLngM);
        Serial.println(UmbralLngm);
        contador = comprobacion;
        Serial.println("comprobacion");
      }

      break;
    case comprobacion:
      if (posLat != gps.location.lat() || posLng != gps.location.lng()) {
        contador = lectura;
        Serial.println("lectura");
      }
      break;
    case lectura:
      if (UmbralLatm < gps.location.lat() < UmbralLatM && UmbralLngm < gps.location.lng() < UmbralLngM) {
        vueltas = vueltas + 1;
        Serial.print("vueltas ");
        Serial.println(vueltas);
        contador = comprobacion;
        Serial.println("comprobacion");
      }
      if (digitalRead(PIN_BTN) == 0) {
        contador = boton;
      }
      break;
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
  //Serial.println(digitalRead(PIN_BTN));

  tft.setCursor(10,50);  // ubica cursor
  tft.print(gps.speed.kmph(),0);
  tft.setTextSize(3);
  tft.print("kmh");
  tft.setTextSize(5);
  tft.setCursor(170,50);  // ubica cursor
  tft.print(posLat,6);    // valor que representa la humedad del sensor en zona 1
  tft.setTextSize(7);

}

void displayInfo(void) {
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {

    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  /*Serial.print(F("  Date/Time: "));
    if (gps.date.isValid())
    {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
    }
    else
    {
    Serial.print(F("INVALID"));
    }*/

  /* Serial.print(F(" "));
    if (gps.time.isValid())
    {
     if (gps.time.hour() < 10) Serial.print(F("0"));
     Serial.print(gps.time.hour());
     Serial.print(F(":"));
     if (gps.time.minute() < 10) Serial.print(F("0"));
     Serial.print(gps.time.minute());
     Serial.print(F(":"));
     if (gps.time.second() < 10) Serial.print(F("0"));
     Serial.print(gps.time.second());
     Serial.print(F("."));
     if (gps.time.centisecond() < 10) Serial.print(F("0"));
     Serial.print(gps.time.centisecond());
    }
    else
    {
     Serial.print(F("INVALID"));
    }*/

  Serial.println();
}
