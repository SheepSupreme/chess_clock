
// SSD1306
#include "oled.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Wire.h Pins
// A4 SDA OLED
// A5 SCL OLED

// Pins
#define CLK 10
#define DATA 11
#define SW 12

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
OLED oled;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 2160)
const int epd_bitmap_allArray_LEN = 5;
const unsigned char* epd_bitmap_allArray[5] = {
  oled.epd_bitmap_Background,
  oled.epd_bitmap_Ebene_1,
  oled.epd_bitmap_Ebene_2_0,
  oled.epd_bitmap_Ebene_2_1,
  oled.epd_bitmap_Ebene_2_2
};


void setup() {
  static int8_t c, val;
  pinMode(CLK, INPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DATA, INPUT);
  pinMode(DATA, INPUT_PULLUP);
  pinMode(SW, INPUT);
  pinMode(SW, INPUT_PULLUP);
  Serial.begin (115200);
  Serial.println("KY-040 Start:");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display();
}



static uint8_t prevNextCode = 0;
static uint16_t store = 0;
int sequenz = 0;



void loop() {
  static int8_t c, val;
  if (digitalRead(SW) == 0) {
    delay(10);
    display.invertDisplay(true);
    while (digitalRead(SW) == 0) {
      delay(10);
    }
    display.invertDisplay(false);
    Serial.println("pressed");
    sequenz = 1;
  }
  if ( val = read_rotary() ) {
    c += val;
    if (sequenz == 0) {
      if (c < 0) {
        c = 2;
      }
      if (c > 2) {
        c = 0;
      }
      Serial.println(c); Serial.print(" ");
      display.clearDisplay();
      display.drawBitmap(0, 0, epd_bitmap_allArray[0], 128 , 64, 1);
      display.drawBitmap(27, 0, epd_bitmap_allArray[1], 74 , 43, 1);

      switch (c) {
        case 0:
          display.drawBitmap(27, 44, epd_bitmap_allArray[2], 74 , 21, 1);
          display.display();
          break;

        case 1:
          display.drawBitmap(27, 44, epd_bitmap_allArray[3], 74 , 21, 1);
          display.display();
          break;

        case 2:
          display.drawBitmap(27, 44, epd_bitmap_allArray[4], 74 , 21, 1);
          display.display();
          break;
      }
    }
  }
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};

  prevNextCode <<= 2;
  if (digitalRead(DATA)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

  // If valid then store as 16 bit data.
  if  (rot_enc_table[prevNextCode] ) {
    store <<= 4;
    store |= prevNextCode;
    //if (store==0xd42b) return 1;
    //if (store==0xe817) return -1;
    if ((store & 0xff) == 0x2b) return -1;
    if ((store & 0xff) == 0x17) return 1;
  }
  return 0;
}
