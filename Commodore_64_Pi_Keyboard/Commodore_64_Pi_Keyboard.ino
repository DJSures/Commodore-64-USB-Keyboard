// ***********************************************************************
// **                                                                   **
// ** Commodore 64 USB HID for Pro Micro                                **
// ** By DJ Sures (Synthiam.com) (c)2019                                **
// **                                                                   **
// ** Updated: November 30, 2019                                        **
// **                                                                   **
// ** This uses the real keys labelled on the C64, not pretending       **
// ** to make a c64 into a pc keyboard. This is if you're wanting a     **
// ** REAL emulated c64.                                                **
// **                                                                   **
// ***********************************************************************

// C64      Arduino
// KeyBoard Pro Micro
//  Pin     Pin
// Rows
//   20     2 - SDA
//   19     3 - SCL
//   18     4 - A6
//   17     5
//   16     6 - A7
//   15     7 -
//   14     8 - A8
//   13     9 - A9

// Columns
//   12     10 - A10
//   11     16 - MOSI
//   10     14 - MISO
//   9      15 - SCLK
//   8      18 - A0
//   7      19 - A1
//   6      20 - A2
//   5      21 - A3
//   4      N/C
//   3      1
//   2      N/C
//   1      0

// Uncomment if you wish to debug keyboard through serial monitor at 9600 baud
//#define DEBUG

#include <HID.h>
#include <Keyboard.h>

int lastKeyDown[80];
long lastDebounceTime[80];
int debounceDelay = 50;
int RowPinMap[8] = {9, 3, 4, 5, 6, 7, 8, 2};
int ColPinMap[10] = {10, 16, 14, 21, 18, 19, 20, 15, 1, 0};

// Keyboard matrix from http://sta.c64.org/cbm64kbdlay.html
uint16_t keyMap[80] = {
  // Del            Return      LR                F7        F1               F3             F5          UD               Null         Null
  KEY_BACKSPACE, KEY_RETURN, KEY_RIGHT_ARROW,  KEY_F7,   KEY_F1,          KEY_F3,        KEY_F5,     KEY_DOWN_ARROW,  NULL,        NULL,

  // 3           W           A                 4         Z                S              E           LSHFT            Joy2Down     Joy1Down
  '3',           'w',        'a',              '4',      'z',             's',           'e',        KEY_LEFT_SHIFT,  '2',         '*',

  // 5           R           D                 6         C                F              T           X                Joy2Left     Joy1Left
  '5',           'r',        'd',              '6',      'c',             'f',           't',        'x',             '4',         '-',

  // 7           Y           G                 8         B                H              U           V                Joy2Right    Joy1Right
  '7',           'y',        'g',              '8',      'b',             'h',           'u',        'v',             '6',         '+',

  // 9           I           J                 Zero      M                K              O           N                Joy2F1       Joy1F1
  '9',           'i',        'j',              '0',      'm',             'k',           'o',        'n',             '5',         KEY_TAB,

  // +           P           L                 -         .                :              @           ,                Joy2F2       Joy1F2
  '+',           'p',        'l',              '-',      '.',             ':',           '@',        ',',             '3',         '1',

  // Pound       *           ;                 Home      RSHFT            =              Pi          /                Restore      Null
  KEY_INSERT,    '*',        ';',              KEY_HOME, KEY_RIGHT_SHIFT, '=',           KEY_DELETE, '/',             KEY_PAGE_UP, NULL,

  //  1          BS          CTRL              2         SPC              C=             Q           RunStop          Joy2Up       Joy1Up
  '1',           KEY_ESC,    KEY_LEFT_CTRL,    '2',      ' ',             KEY_F12,       'q',        KEY_ESC,         '8',         '\\',
};

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
#else
  Keyboard.begin();
#endif

  for (int i = 0; i < 80; i++)
    lastKeyDown[i] = 0;

  for (int Row = 0; Row < 8; Row++)
    pinMode(RowPinMap[Row], INPUT_PULLUP);

  for (int Col = 0; Col < 10; Col++)
    pinMode(ColPinMap[Col], INPUT_PULLUP);
}


void loop() {

  for (int Row = 0; Row < 8; Row++) {

    int RowPin = RowPinMap[Row];

    pinMode(RowPin, OUTPUT);

    digitalWrite(RowPin, LOW);

    for (int Col = 0; Col < 10; Col++) {

      int keyPos = Col + (Row * 10);

      if (lastDebounceTime[keyPos] + debounceDelay < millis()) {

        int outChar   = keyMap[keyPos];
        int isKeyDown = !digitalRead(ColPinMap[Col]);

        // shift key
        if (lastKeyDown[17]) {

          if (outChar == '2') {
            
            outChar = '"';
          } else if (outChar == '6') {
            
            outChar = '&';
          } else if (outChar == '7') {
            
            outChar = '\'';
          } else if (outChar == ';') {
            
            outChar = ']';
            Keyboard.release(keyMap[17]);
          } else if (outChar == ':') {
            
            outChar = '[';
            Keyboard.release(keyMap[17]);
          }
        }

        if (isKeyDown && !lastKeyDown[keyPos]) {

          lastKeyDown[keyPos] = true;

          lastDebounceTime[keyPos] = millis();

#ifndef DEBUG
          Keyboard.press(outChar);
#else
          Serial.print("col: ");
          Serial.print(Col);
          Serial.print(", row: ");
          Serial.print(Row);
          Serial.print(", Position: ");
          Serial.print(keyPos);
          Serial.print(", char:");
          Serial.print(outChar);
          Serial.println();
#endif
        }

        if (!isKeyDown && lastKeyDown[keyPos]) {

          lastKeyDown[keyPos] = false;

          lastDebounceTime[keyPos] = millis();

#ifndef DEBUG
          Keyboard.release(outChar);
#else
          Serial.print("Released ");
          Serial.print(keyPos);
          Serial.println();
#endif
        }
      }
    }

    digitalWrite(RowPin, HIGH);  

    delay(1);                    

    pinMode(RowPin, INPUT_PULLUP); 
  }
}
