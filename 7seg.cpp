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
}

void MAX2719::setPlayerTime(int sec1, int sec2) {
  playerTime[0] = sec1;
  playerTime[1] = sec2;
  // Zeit beider Spieler anzeigen
  displayTime(0, playerTime[0]);
  displayTime(1, playerTime[1]);
}

void MAX2719::displaySegment(int pos, int data) {
  data += (9 - pos) * 0x0100;
  sendMux(data);
}

void MAX2719::displayTime(bool leftPos, int seconds) {
  int minutes = (seconds / 60) % 100; // maximale Minutenanzahl = 99
  int remainingSeconds = seconds % 60;

  int result = minutes * 100 + remainingSeconds; // Anzeige = [minutes].[seconds]
  displayInt(leftPos, result);
}

void MAX2719::displayInt(bool leftPos, int num) {
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
    int displayPos = i + 1; // displayPos: 1 steht für die erste MAX2719-Anz, 8 für die letzte
    if (leftPos) {
      displayPos += maxDigits; // Für linke Seite
    }
    if (i < count) {
      int data = segment.numbers[digits[i]]; // Ziffer
      displaySegment(displayPos, data);
    } else {
      int data = 0; // clear
      displaySegment(displayPos, data);
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
  gameState = GameOver;
  for (int i = 1; i < 5; i++) {
    int pos = i;
    if (lostPlayer == 1) {
      pos += 4; // für rechte seite
    }
    displaySegment(pos, segment.numbers[0]);
  }
}

void MAX2719::timerLogic() {
  if (gameState == Menu || gameState == Paused || gameState == Ready) {
    return;
  }

  if (gameState == GameOver) {
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

  if (gameState == Playing) {
    playerTime[currentPlayer]--;
    displayTime(currentPlayer, playerTime[currentPlayer]);

    // check ob jemand verloren hat
    if (playerTime[currentPlayer] <= 0) {
      gameOver(currentPlayer);
    }
  }
}

void MAX2719::loop() {
  if (gameState == Ready) {
    if (!digitalRead(BUTTON0)) {
      currentPlayer = 1;
      gameState = Playing;
    } else if (!digitalRead(BUTTON1)) {
      currentPlayer = 0;
      gameState = Playing;
    }
  }

  if (gameState == Playing) {
    if (currentPlayer == 0 && !digitalRead(BUTTON0) || currentPlayer == 1 && !digitalRead(BUTTON1)) {
      currentPlayer = !currentPlayer;
    }
  }
}
