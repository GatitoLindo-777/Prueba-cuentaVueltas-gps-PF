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

#define TFT_DC 4
#define TFT_CS 15
#define TFT_RST 2
#define TFT_MISO 19         
#define TFT_MOSI 23           
#define TFT_CLK 18
#define RXD2 16
#define TXD2 17

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  
  tft.begin();
  tft.setRotation(3);       // establece posicion vertical con pines hacia abajo
  tft.fillScreen(ILI9341_BLACK);    // fondo de pantalla de color negro
  tft.fillRect(0, 0, 320, 30, ILI9341_NAVY);  // rectangulo azul naval a modo de fondo de titulo 
  tft.setTextColor(ILI9341_WHITE);    // color de texto en blanco
  tft.setTextSize(2);       // escala de texto en 2
  tft.setCursor(82,10);        // ubica cursor
  tft.print("TRACK X TRACK");    // imprime texto
  

}

void loop() {

  tft.setCursor(10,40);  
  tft.print("LAT:");    
  tft.setTextSize(2);
  tft.print(posLat,6);
  tft.setCursor(10,60);  
  tft.print("LON:");   
  tft.setTextSize(2);
  tft.print(posLng,6);
  tft.setCursor(10,80);  
  tft.print("ULAT+:");    
  tft.setTextSize(2);
  tft.print(UmbralLatM,6);
  tft.setCursor(10,100);  
  tft.print("ULAT-:");   
  tft.setTextSize(2);
  tft.print(UmbralLatm,6);
  tft.setCursor(10,120);  
  tft.print("ULON+:);    
  tft.setTextSize(2);
  tft.print(UmbralLngM,6);
  tft.setCursor(10,140);  
  tft.print("ULON-:");    
  tft.setTextSize(2);
  tft.print(UmbralLngm,6)
  tft.setCursor(10,160);  
  tft.print("VPOR:");    
  tft.setTextSize(2);
  tft.print(Vpor);
  tft.setCursor(10,180);  
  tft.print("VUELTAS: ");    
  tft.setTextSize(2);
  tft.print(vueltas);
  tft.setCursor(10,200);  
  tft.print("VEL:");
  tft.setTextSize(2);
  tft.print(gps.speed.kmph(),0);

}
