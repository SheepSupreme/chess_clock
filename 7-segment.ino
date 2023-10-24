//--------------------------------------------------------------------
// Projekt: Schachuhr
// Funktion: MUX7219 7-Segment-Anzeige
// Programmersteller: David KEDRA
//--------------------------------------------------------------------

//------------------ Pins --------------------------------------------
#define CLK 13
#define CS 12
#define DATA 11
#define BUTTON0 9
#define BUTTON1 10

//------------------ Variablen ---------------------------------------
int playerTime[2];

bool currentPlayer = 0;
bool timerActive = true;

struct Segment {
  uint8_t numbers[10] = {
    0b01111110, // 0
    0b00110000, // 1
    0b01101101, // 2
    0b01111001, // 3
    0b00110011, // 4
    0b01011011, // 5
    0b01011111, // 6
    0b01110000, // 7
    0b01111111, // 8
    0b01111011, // 9
  };
  uint8_t letters[10] = {

  };
};
Segment segment;

//------------------ Unterprogramme ----------------------------------

// Interrupt Service Routine
ISR(TIMER1_COMPA_vect)
{
  if (!timerActive) {
    return;
  }

  playerTime[currentPlayer]--;

  displaySegmentInt(currentPlayer, playerTime[currentPlayer]);

  // check ob jemand verloren hat
  if (playerTime[currentPlayer] <= 0) {
    gameOver(currentPlayer);
  }
}

void timerSetup() {
  // Timer1 im CTC-Modus auf 1 Sekunde einstellen
  noInterrupts(); // alle Interrupts abschalten cli()
  TCCR1A = 0; // Registereintrag löschen
  TCCR1B = 0; // Registereintrag löschen
  TCCR1B |= (1 << CS12); // 256 als Prescale-Wert spezifizieren
  TCCR1B |= (1 << WGM12); // Setzt das Bit für den CTC-Mode
  OCR1A = 62499; // Auf Endwert setzen 62500 Impulse zählen = 1 Sekunde
  TIMSK1 |= (1 << OCIE1A); // Schaltet den Compare-Interrupt ein
  interrupts(); // Alle Interrupts scharf schalten sei()
}

void displaySetup() {
  sendMux(0x0B07); // Scan Limit Register
  delay(10);
  sendMux(0x0A0F); // Intensity (Helligkeit)
  delay(10);
  sendMux(0x0C01); // Shutdown Register
  delay(10);
  sendMux(0x0900); // Decode Mode
  delay(10);

  clearDisplay();
}

/*
  Anzeige von `data` auf der 7Seg-Anz an der Position `pos`.

  pos: 1 steht für die erste 7Seg-Anz, 8 für die letzte.
*/
void displaySegment(int pos, int data) {
  data += (9 - pos) * 0x0100;
  sendMux(data);
}

/*
  Anzeige einer Zahl auf einer Seite der 7Seg-Anz.

  leftPos: true, um die Zahl auf den ersten vier 7Seg-Anz darzustellen.
  leftPos: false, um die Zahl auf den 5. bis 8. 7Seg-Anz darzustellen.

  num: Die Zahl die angezeigt werden soll (maximal 4 Ziffern).
*/
void displaySegmentInt(bool leftPos, int num) {
  const int maxDigits = 4; // 4 7Seg-Anz pro Seite
  int digits[maxDigits];

  int count = 0;

  // Jede Ziffer einzeln speichern
  while (num > 0 && count < maxDigits) {
    int digit = num % 10; // Letzte Ziffer der Zahl
    digits[count] = digit;
    count++;
    num /= 10; // Letzte Ziffer entfernen
  }

  // Reverse array
  for (int i = 0; i < count / 2; i++) {
    int temp = digits[i];
    digits[i] = digits[count - 1 - i];
    digits[count - 1 - i] = temp;
  }

  // Anzeige
  for (int i = 0; i < 4; i++) {
    int displayPos = i + 1;
    if (!leftPos) {
      displayPos += maxDigits; // Für Rechte Seite
    }
    if (i < count) {
      displaySegment(displayPos, segment.numbers[digits[i]]); // Ziffer anzeigen
    } else {
      displaySegment(displayPos, 0); // clear
    }
  }
}

// Kommunikation mit MUX7219
void sendMux(unsigned int data) {
  digitalWrite(CS, LOW);
  bool zw;
  unsigned int maske = 0x8000;

  maske = 0x8000;
  for (int i = 0; i < 16; i++) {
    digitalWrite(CLK, LOW);
    zw = (data & maske) == maske;
    maske = maske >> 1;
    digitalWrite(DATA, zw);
    digitalWrite(CLK, HIGH);
  }
  digitalWrite(CS, HIGH);
}

// Anzeige löschen
void clearDisplay() {
  for (int i = 1; i < 9; i++) {
    displaySegment(i, 0);
  }
}

void gameOver(int lostPlayer) {
  timerActive = false;
  for (int i = 1; i < 9; i++) {
    displaySegment(i, segment.numbers[lostPlayer]);
  }
}

//-------------------- Setup -----------------------------------------
void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DATA, OUTPUT);

  Serial.begin(9600);
  Serial.println("start");

  playerTime[0] = 10;
  playerTime[1] = 10;
  
  timerSetup();
  displaySetup();
}

//------------------- Loop -------------------------------------------
void loop() {
  if (currentPlayer == 0 && digitalRead(BUTTON0) || currentPlayer == 1 && digitalRead(BUTTON1)) {
    currentPlayer = !currentPlayer;
  }
}

//--------------------------------------------------------------------
