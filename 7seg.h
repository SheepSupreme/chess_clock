#include <stdint.h>
#include <Arduino.h>

class MAX2719 {
  private:
    //------------------ Variablen -----------------------------------
    int playerTime[2];

    bool currentPlayer = 1;
    bool isTimerActive = true;
    bool isGameOver = false;
    bool isGameRunning = false;

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
      uint8_t letters[10] = {

      };
    };
    Segment segment;

  public:
    // Pins
    const int CLK = 7;
    const int CS = 6;
    const int DATA = 5;
    const int BUTTON1 = 9;
    const int BUTTON0 = 8;

    // Setup
    void segmentSetup();

    /*
      Anzeige von `data` auf der MAX2719-Anz an der Position `pos`.

      pos: 1 steht für die erste MAX2719-Anz, 8 für die letzte.
    */
    void displaySegment(int pos, int data);

    /*
      Anzeige einer Zahl auf einer Seite der MAX2719-Anz.

      leftPos: true, um die Zahl auf den ersten vier MAX2719-Anz darzustellen.
      leftPos: false, um die Zahl auf den 5. bis 8. MAX2719-Anz darzustellen.

      num: Die Zahl die angezeigt werden soll (maximal 4 Ziffern).
    */
    void displaySegmentInt(bool leftPos, int num);

    // Kommunikation mit MUX7219
    void sendMux(unsigned int data);

    // Anzeige löschen
    void clearDisplay();

    // wird jede Sekunde ausgeführt
    void timerLogic();

    // Zeit eines Spielers ist ausgegangen
    void gameOver(bool lostPlayer);

    void loop();
};
