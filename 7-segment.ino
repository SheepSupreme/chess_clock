//--------------------------------------------------------------------
// Projekt: Schachuhr
// Funktion: MUX7219 7-Segment-Anzeige
// Programmersteller: David KEDRA
//--------------------------------------------------------------------

#include "7seg.h"

//------------------ Pins --------------------------------------------

//------------------ Variablen ---------------------------------------

MAX2719 max2719;

//------------------ Unterprogramme ----------------------------------

// Interrupt Service Routine
ISR(TIMER1_COMPA_vect)
{
  max2719.timerLogic();
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

//-------------------- Setup -----------------------------------------
void setup() {
  pinMode(max2719.CLK, OUTPUT);
  pinMode(max2719.CS, OUTPUT);
  pinMode(max2719.DATA, OUTPUT);
  
  pinMode(max2719.BUTTON0, INPUT);
  pinMode(max2719.BUTTON1, INPUT);

  Serial.begin(9600);
  Serial.println("start");

  timerSetup();
  max2719.segmentSetup();
}

//------------------- Loop -------------------------------------------
void loop() {
  max2719.loop();
}

//--------------------------------------------------------------------
