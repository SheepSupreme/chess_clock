//--------------------------------------------------------------------
// modul_klassen_test
//--------------------------------------------------------------------

#define CLK 13
#define CS 12
#define DATA 11
#define BUTTON0 9
#define BUTTON1 10

int time0 = 300;
int time1 = 600;

bool currentPlayer = 0;




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

//------------------ Unterprogramme ---------------------------------

void displaySegment(int pos, int data) {
  data += (9 - pos) * 0x0100;
  senden(data);
}

void displaySegmentInt(bool leftPos, int num) {
    const int maxDigits = 4;  // Maximum number of digits
    int digits[maxDigits];     // Array to store digits

    int count = 0;  // Counter for the number of digits

    while (num > 0 && count < maxDigits) {
        int digit = num % 10; // Get the last digit
        digits[count] = digit;
        count++;
        num /= 10; // Remove the last digit
    }

    // Reverse the digits in the array
    for (int i = 0; i < count / 2; i++) {
        int temp = digits[i];
        digits[i] = digits[count - 1 - i];
        digits[count - 1 - i] = temp;
    }

    for (int i = 0; i < count; i++) {
        int displayPos = i + 1;
        if (!leftPos) {
          displayPos += 4;
        }
        displaySegment(displayPos, segment.numbers[digits[i]]);
    }
}


void senden(unsigned int data) {
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

void clearDisplay() {
  for (int j = 1; j < 9; j++) {
    displaySegment(j, 0x0000);
  }
}

//-------------------- Setup -----------------------------------------

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DATA, OUTPUT);
  unsigned int wert;

  Serial.begin(9600);
  Serial.println("start");

  wert = 0x0B07;
  senden(wert); // Scan Limit Register

  wert = 0x0A0F;
  senden(wert); // Intensity

  wert = 0x0C01;
  senden(wert); // Shutdown Register

  wert = 0x0900;
  senden(wert); // Decode Mode


  // Anzeige clear
  for (int j = 1; j < 9; j++) {
    wert = 0x0000;
    displaySegment(j, wert);
  }
}

//------------------- Loop -------------------------------------------

void loop() {

  if (currentPlayer == 0 && digitalRead(BUTTON0) || currentPlayer == 1 && digitalRead(BUTTON1)) {
    currentPlayer = !currentPlayer;
  }
  
  clearDisplay();
  displaySegmentInt(0, time0);
  displaySegmentInt(1, time1);
  if (currentPlayer == 0) {
    time0--;
  }
  if (currentPlayer == 1) {
    time1--;
  }
  delay(1000);
}

//--------------------------------------------------------------------
