// Made by David Kedra & Lukas Gregor
// chess_clock.ino

// Include eigener header Dateien
#include "7seg.h"
#include "oled.h"

// Include weiterer Bibliotheken für die Verwendung des OLED-Displays
#include <SPI.h>
#include <Wire.h> // Erkennung des Arduinos und automatische dekleration der SDA und SCL Pins
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

MAX2719 max2719;

// Pins
const int CLK = 10;
const int DATA = 11;
const int SW = 12;

// Einstellungen für OLED-Display
#define SCREEN_WIDTH 128  // OLED display Weite, in Pixel
#define SCREEN_HEIGHT 64  // OLED display Höhe, in Pixel
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static uint8_t prevNextCode = 0;
static uint16_t store = 0;

// Timer: ein Mal pro Sekunde aufgerufen
ISR(TIMER1_COMPA_vect) {
  max2719.timerLogic();
}

void timerSetup() {
  // Timer1 im CTC-Modus auf 1 Sekunde einstellen
  noInterrupts();           // alle Interrupts abschalten cli()
  TCCR1A = 0;               // Registereintrag löschen
  TCCR1B = 0;               // Registereintrag löschen
  TCCR1B |= (1 << CS12);    // 256 als Prescale-Wert spezifizieren
  TCCR1B |= (1 << WGM12);   // Setzt das Bit für den CTC-Mode
  OCR1A = 62499;            // Auf Endwert setzen 62500 Impulse zählen = 1 Sekunde
  TIMSK1 |= (1 << OCIE1A);  // Schaltet den Compare-Interrupt ein
  interrupts();             // Alle Interrupts scharf schalten sei()
}

// Array aller Bitmaps für Übersicht
const int epd_bitmap_allArray_LEN = 5;
const unsigned char* epd_bitmap_allArray[8] = {
  epd_bitmap_Background,
  epd_bitmap_Ebene_1,
  epd_bitmap_Ebene_2_0,
  epd_bitmap_Ebene_2_1,
  epd_bitmap_Ebene_2_2,
  Pause,
  schwarz,
  white
};

// Unterprogramm, dass einen Knopfdruck des Rotary Encoders erkennt
// Auf dem OLED wird eine Farbenwechsel als visueler Indikator durchgeführt
bool click() {
  if (digitalRead(SW) == 0) {
    delay(10);
    display.invertDisplay(true);
    while (digitalRead(SW) == 0) {
      delay(10);
    }
    display.invertDisplay(false);
    return true;
  }
  return false;
}

// Unterprogramm für Modi Auswahl im Menü
int mode_select() {
  static int8_t c, val;
  while (!click()) {
    if (val = read_rotary()) {
      c += val;
      if (c == -1) { c = 2; }
      c = c % 3;
      Serial.println(c);
      display.clearDisplay();
      display.drawBitmap(0, 0, epd_bitmap_allArray[0], 128, 64, 1);
      display.drawBitmap(27, 0, epd_bitmap_allArray[1], 74, 43, 1);
      display.drawBitmap(27, 44, epd_bitmap_allArray[c + 2], 74, 21, 1);
      display.display();
    }
  }
  return c;
}

// Unterprogramm für Software debounce des Rotary Encoders
int8_t read_rotary() {
  static int8_t rot_enc_table[] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 }; // Table für erlaubte und unerlaubte Zustände

  prevNextCode <<= 2;
  if (digitalRead(DATA)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

  // If valid then store as 16 bit data.
  if (rot_enc_table[prevNextCode]) {
    store <<= 4;
    store |= prevNextCode;
    //if (store==0xd42b) return 1;
    //if (store==0xe817) return -1;
    if ((store & 0xff) == 0x2b) return -1;
    if ((store & 0xff) == 0x17) return 1;
  }
  return 0;
}

void setup() {
  static int8_t c, val, time1, time2;
  c = -1;

  pinMode(max2719.CLK, OUTPUT);
  pinMode(max2719.CS, OUTPUT);
  pinMode(max2719.DATA, OUTPUT);

  pinMode(max2719.BUTTON0, INPUT_PULLUP);
  pinMode(max2719.BUTTON1, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("start");

  timerSetup();
  max2719.segmentSetup();

  max2719.setPlayerTime(0, 0);

  pinMode(CLK, INPUT_PULLUP);
  pinMode(DATA, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  display.display();

  // Modi Auswahl
  while (c < 0) { c = mode_select(); }

  // Zeiteinstellung abhängig von dem Ausgewählten Modus
  switch (c) {
    case 0:  //10 Minuten
      max2719.setPlayerTime(10 * 60, 10 * 60);
      max2719.gameState = max2719.Ready;
      break;
    case 1: // Manuelle Zeiteinstellung
      while (!click()) {
        if (val = read_rotary()) {
          time1 += val;
        }
        max2719.setPlayerTime(time1 * 10, 0);
      }
      while (!click()) {
        if (val = read_rotary()) {
          time2 += val;
        }
        max2719.setPlayerTime(time1 * 10, time2 * 10);
      }

      max2719.gameState = max2719.Ready;
      break;
    case 2: // 5 Minuten
      max2719.setPlayerTime(5 * 60, 5 * 60);
      max2719.gameState = max2719.Ready;
      break;
  }
  display.clearDisplay();
}

void loop() {
  max2719.loop(); // Gametimer
  display.clearDisplay(); // Oled Display clear
  if (max2719.gameState == max2719.Playing) {
    display.drawBitmap(27, 0, epd_bitmap_allArray[1], 74, 43, 1); // display von Standard Bitmap
    display.invertDisplay(max2719.getCurrentPlayer() == max2719.getWhitePlayer()); // Oled weiß(wenn Weiß am Zug)& schwarz(wenn Schwarz am Zug)
    if(max2719.getCurrentPlayer() == max2719.getWhitePlayer()){
      display.drawBitmap(25, 44, epd_bitmap_allArray[7], 74, 50, 1); // Bitmap für Spieler der am Zug ist
    }
    else {display.drawBitmap(25, 44, epd_bitmap_allArray[6], 74, 50, 1);} // Bitmap für Spieler der am Zug ist
    display.display();
    if (click()) {
      max2719.gameState = max2719.Paused; // Bei Knopfdruck des Rotary Encoders wird das Spiel pausiert
    }
  } 
  else if (max2719.gameState == max2719.Paused) {
    display.clearDisplay();
    display.drawBitmap(0, 0, epd_bitmap_allArray[5], 128, 64, 1); // Oled zeigt die Pause an
    display.display();
    if (click()) {
      max2719.gameState = max2719.Playing; // Bei Knopfdruck des Rotary Encoders wird das Spiel fortgesetzt
    }
  }
}
