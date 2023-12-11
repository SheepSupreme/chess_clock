#include "7seg.h"

void MAX2719::segmentSetup() {
  // display setup
  sendMux(0x0B07); // Scan Limit Register
  delay(10);
  sendMux(0x0A0F); // Intensity (Helligkeit)
  delay(10);
  sendMux(0x0C01); // Shutdown Register
  delay(10);
  sendMux(0x0900); // Decode Mode
  delay(10);

  clearDisplay();

  
  playerTime[0] = 4;
  playerTime[1] = 4;

  // Zeit beider Spieler anzeigen
  displaySegmentInt(0, playerTime[0]);
  displaySegmentInt(1, playerTime[1]);

  isGameRunning = true;
}

void MAX2719::displaySegment(int pos, int data) {
  data += (9 - pos) * 0x0100;
  sendMux(data);
}

void MAX2719::displaySegmentInt(bool leftPos, int num) {
  const int maxDigits = 4; // 4 MAX2719-Anz pro Seite
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
    if (leftPos) {
      displayPos += maxDigits; // Für linke Seite
    }
    if (i < count) {
      displaySegment(displayPos, segment.numbers[digits[i]]); // Ziffer anzeigen
    } else {
      displaySegment(displayPos, 0); // clear
    }
  }
}

void MAX2719::sendMux(unsigned int data) {
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

void MAX2719::clearDisplay() {
  for (int i = 1; i < 9; i++) {
    displaySegment(i, 0);
  }
}

void MAX2719::gameOver(bool lostPlayer) {
  isGameOver = true;
  isGameRunning = false;
  for (int i = 1; i < 5; i++) {
    int pos = i;
    if (lostPlayer == 1) {
      pos += 4; // für rechte seite
    }
    displaySegment(pos, segment.numbers[0]);
  }
}

void MAX2719::timerLogic() {
  if (!isTimerActive) {
    return;
  }

  if (isGameOver) {
    if (gameOverBlink_On) {
      // Nuller nicht zeigen
      for (int i = 1; i < 5; i++) {
        int pos = i;
        if (currentPlayer == 1) {
          pos += 4; // für rechte seite
        }
        displaySegment(pos, 0);
      }
    } else {
      // Nuller Anzeigen auf Position des verlorenen Spielers
      for (int i = 1; i < 5; i++) {
        int pos = i;
        if (currentPlayer == 1) {
          pos += 4; // für rechte seite
        }
        displaySegment(pos, segment.numbers[0]);
      }
    }
    gameOverBlink_On = !gameOverBlink_On;
    return;
  }

  if (isGameRunning) {
    playerTime[currentPlayer]--;
    displaySegmentInt(currentPlayer, playerTime[currentPlayer]);

    // check ob jemand verloren hat
    if (playerTime[currentPlayer] <= 0) {
      gameOver(currentPlayer);
    }
  }
}

void MAX2719::loop() {
  if (isGameRunning && !isGameOver && isTimerActive) {
    if (currentPlayer == 0 && digitalRead(BUTTON0) || currentPlayer == 1 && digitalRead(BUTTON1)) {
      currentPlayer = !currentPlayer;
    }
  }
}
