/*********************************************************************
This is a library for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#ifndef _ADAFRUIT_PCD8544_H
#define _ADAFRUIT_PCD8544_H

// reduces how much is refreshed, which speeds it up!
// originally derived from Steve Evans/JCW's mod but cleaned up and
// optimized
#define enablePartialUpdate

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
#endif

#include <SPI.h>

#define USE_FAST_PINIO

#ifdef __SAM3X8E__
  typedef volatile RwReg PortReg;
  typedef uint32_t PortMask;
#elif defined (ESP8266) || defined (__STM32F1__) || defined (ESP32)
  typedef volatile uint32_t PortReg;
  typedef uint32_t PortMask;
#else
  typedef volatile uint8_t PortReg;
  typedef uint8_t PortMask;
#endif


#define BLACK 1
#define WHITE 0

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

#define SCROLL_LEFT 1
#define SCROLL_RIGHT 2
#define SCROLL_UP 3
#define SCROLL_DOWN 4

// Default to max SPI clock speed for PCD8544 of 4 mhz (16mhz / 4) for normal Arduinos.
// This can be modified to change the clock speed if necessary (like for supporting other hardware).
#define PCD8544_SPI_CLOCK_DIV SPI_CLOCK_DIV4

class Adafruit_PCD8544 : public Adafruit_GFX {
 public:
  // Software SPI with explicit CS pin.
  Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST);
  // Software SPI with CS tied to ground.  Saves a pin but other pins can't be shared with other hardware.
  Adafruit_PCD8544(int8_t SCLK, int8_t DIN, int8_t DC, int8_t RST);
  // Hardware SPI based on hardware controlled SCK (SCLK) and MOSI (DIN) pins. CS is still controlled by any IO pin.
  //Adafruit_PCD8544(int8_t DC, int8_t CS, int8_t RST);
  Adafruit_PCD8544(int8_t DC, int8_t CS, int8_t RST, SPIClass *useSPI = &SPI);
  
  void begin(uint8_t contrast = 40, uint8_t bias = 0x04);

  void updateBoundingBox(uint8_t xmin, uint8_t ymin, uint8_t xmax, uint8_t ymax);
    
  void command(uint8_t c);
  void data(uint8_t c);

  void setContrast(uint8_t val);
  void clearDisplay(uint8_t color = 0);
  void display();
  void invertDisplay(boolean i);
  // Return the address of the raw buffer for application-side processing
  uint8_t * getPixelBuffer();
  // Enable/disable power-saving mode, ie. turn the display off/on
  void powerSaving(boolean i);
  void scroll(uint8_t direction = SCROLL_UP, uint8_t pixels = 1, uint8_t fillColor = WHITE);

  void drawPixel(int16_t x, int16_t y, uint16_t color);
  uint8_t getPixel(int8_t x, int8_t y);
  void clearDisplayRAM(uint8_t color=0);

 private:
  int8_t _din, _sclk, _dc, _rst, _cs;
  volatile PortReg *mosiport, *clkport, *dcport, *csport;
  PortMask mosipinmask, clkpinmask, cspinmask, dcpinmask;
  void spi_begin();
  void spi_end();
  void spiWrite(uint8_t c);
  bool isHardwareSPI();
  SPIClass *_SPI = &SPI;
};

#endif
