#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
void displayInfo();

#define PIN_BTN 2
double posLat;
double posLng;

double UmbralLatM; //lat+
double UmbralLatm; //Lat-
double UmbralLngM; //Lng+
double UmbralLngm; //Lng-

#define VUmbral 0.0000003

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
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  pinMode (PIN_BTN, INPUT_PULLUP);

  /*Serial.println(F("DeviceExample.ino"));
    Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
    Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();*/
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  switch (contador) {
    case boton:
      if (digitalRead(PIN_BTN) == 0) {
        posLat = gps.location.lat();
        posLng = gps.location.lng();
        Serial.print("posLat");
        Serial.println(posLat, 6);
        Serial.print("posLng");
        Serial.println(posLng, 6);

        UmbralLatM = posLat + VUmbral;
        UmbralLatm = posLat - VUmbral;
        UmbralLngM = posLng + VUmbral;
        UmbralLngm = posLat - VUmbral;
        
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

}
void displayInfo()
{
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
