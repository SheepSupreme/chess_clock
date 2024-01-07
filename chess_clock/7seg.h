// Made by David Kedra
// 7seg.h

#include <stdint.h>
#include <Arduino.h>

class MAX2719 {
  private:
    //------------------ Variablen -----------------------------------
    int playerTime[2];

    bool currentPlayer = 0;
    bool whitePlayer = 0;

    bool gameOverBlink_On = true;

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
    };
    Segment segment;

  public:
    enum GameState {
      Menu,     // Bevor dem Spiel und im Menü
      Playing,  // Spiel läuft
      Ready,    // Zeit eingestellt und bereit zum Spielen (Der nächste Spieler der seinen Knopf drückt, beginnt das Spiel, indem die Zeit seines Gegners beginnt abzulaufen)
      Paused,   // Pausiert (Im Pause-Menü während dem Spiel)
      GameOver  // Nach Spiel, nicht im Menü
    };

    // Pins
    const int CLK = 7;
    const int CS = 6;
    const int DATA = 5;
    const int BUTTON1 = 9;
    const int BUTTON0 = 8;

    GameState gameState = Menu;

    // Getter für currentPlayer
    bool getCurrentPlayer() {
      return currentPlayer;
    }

    bool getWhitePlayer() {
      return whitePlayer;
    }

    // Setup
    void segmentSetup();

    // Die Zeit beider Spieler seperat einstellen
    void setPlayerTime(int sec1, int sec2);

    // Kommunikation mit MUX7219
    void sendMux(unsigned int data);

    // and der Position pos data Anzeigen
    void displaySegment(int pos, int data);

    // Zeigt die Zeit in Minuten und Sekunden an mit Anwendung von der Funktion displayInt().
    void displayTime(bool leftPos, int seconds);

    /*
      Anzeige einer Zahl auf einer Seite der MAX2719-Anz.

      leftPos: true, um die Zahl auf den ersten vier MAX2719-Anz darzustellen.
      leftPos: false, um die Zahl auf den 5. bis 8. MAX2719-Anz darzustellen.

      num: Die Zahl die angezeigt werden soll (maximal 4 Ziffern).
    */
    void displayInt(bool leftPos, int num);

    // Anzeige löschen
    void clearDisplay();

    // wird jede Sekunde ausgeführt
    void timerLogic();

    // Zeit eines Spielers ist ausgegangen
    void gameOver(bool lostPlayer);

    // Soll im Arduino loop ausgeführt werden
    // Taster bezogener Code
    void loop();
};
