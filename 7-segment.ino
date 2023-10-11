//--------------------------------------------------------------------
// modul_klassen_test
//--------------------------------------------------------------------

#define CLK 13
#define CS 10
#define DATA 11

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

//--------------------------------------------------------------------

void display(int pos, int data) {
  data += (9 - pos) * 0x0100;
  senden(data);
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

void ClearDisplay() {
  for (int j = 1; j < 9; j++) {
    display(j, 0x0000);
  }
}

//--------------------------------------------------------------------

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DATA, OUTPUT);
  unsigned int wert;

  Serial.begin(9600);
  Serial.println("start");

  wert = 0x0B07;
  senden(wert); // Scan Limit Register

  wert = 0x0A00;
  senden(wert); // Intensity

  wert = 0x0C01;
  senden(wert); // Shutdown Register

  wert = 0x0900;
  senden(wert); // Decode Mode


  // Anzeige clear
  for (int j = 1; j < 9; j++) {
    wert = 0x0000;
    display(j, wert);
  }


}

//--------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <algorithm>

// !get single digits of integer!
// 
//int main() {
//    int iNums = 12476;
//    std::vector<int> digits;
//
//    while (iNums > 0) {
//        int digit = iNums % 10; // Get the last digit
//        digits.push_back(digit);
//        iNums /= 10; // Remove the last digit
//    }
//
//    std::reverse(digits.begin(), digits.end());
//
//    for (int digit : digits) {
//        // Do something with the digit (it's an integer)
//        std::cout << digit; // For example, print it
//    }
//
//    return 0;
//}


int number = 0;
int time1 = 600;
int time2 = 600;

void loop() {
  display(1, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(2, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(3, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(4, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(5, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(6, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(7, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  display(8, segment.numbers[number]); 
  delay(100);
  ClearDisplay();
  number++;
}

//--------------------------------------------------------------------
