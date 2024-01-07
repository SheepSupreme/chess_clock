# Bedienungsanleitung
## Menüauswahl:
Das OLED-Display zeigt das Hauptmenü an, in dem Sie die Einstellungen, wie die Zeiteinstellungen vornehmen können.
Verwenden Sie den Rotary Encoder, um das Menü zu durchlaufen.
Drücken Sie den Rotary Encoder, um eine Option auszuwählen.
## Zeiteinstellungen:
Im Menü können Sie die Zeit für das Spiel pro Spieler einstellen. Die Standardzeit beträgt 10 Minuten oder 5 Minuten pro Spieler.
Bei Auswahl der manuellen Einstellung verwenden Sie den Rotary Encoder, um die Minuten- und Sekundenwerte zu ändern.
Drücken Sie den Rotary Encoder, um die Auswahl zu bestätigen.
## Spiel starten:
Nachdem Sie die Zeiteinstellungen vorgenommen haben, beginnt die Zeit abzulaufen, sobald ein Spieler seinen Taster betätigt hat.
## Spielverlauf:
Der Spieler, der nicht den Taster betätigt hat, beginnt mit seinem Zug.
Die Zeit für beide Spieler wird auf dem 7-Segment-Display angezeigt: Die verbleibenden Minuten, gefolgt von einem Punkt und den verbleibenden Sekunden.
## Zug beenden:
Jeder Spieler hat einen eigenen Taster. Um Ihren Zug zu beenden, drücken Sie Ihren Taster. Der Zug des anderen Spielers beginnt und seine Zeit fängt an abzulaufen.
## Spielende:
Zeitablauf: Wenn die Zeit für einen Spieler abgelaufen ist, wird das Spiel beendet. Die Zeitanzeige des verlorenen Spielers beginnt zu blinken. 
Um ein neues Spiel zu starten, betätigen Sie den Arduino-Reset-Taster.

| Name            | Eigenschaft                       | Stück | Anwendung               |
|-----------------|-----------------------------------|-------|-------------------------|
| Anzeigemodul    | 7-Segment mit MAX7219             | 1     | Anzeige der Zeit        |
| Rotary Encoder  | KY040                             | 1     | Menü-Steuerung          |
| OLED-Display    | I2C, 128x64, 0,96 Zoll            | 1     | Anzeige                 |
| Taster          | NO, Rot                           | 1     | 1. Spieler              |
| Taster          | NO, Schwarz                       | 1     | 2. Spieler              |
| Arduino         | Nano                              | 1     | Prozessor               |
| Widerstand      | 100k                              | 2     | Pull-Down für Taster    |

1.2	I/O-Belegung

Alle Pins sind bezogen auf ein Arduino Mega.
7-Segment Anzeige:
5 (Data), 6 (CS), 7 (CLK)
Taster:
8 (Taster 0), 9 (Taster 1)
Rotary Encoder: 
10 (CLK), 11 (DATA), 12 (SW)
OLED-Display:
20 (SDA), 21 (SCL)
