#include "Joystick.h"

// ***********************************************************************
// **                                                                   **
// ** Joystick USB HID for Teensy or Pro Micro                          **
// ** By DJ Sures (Synthiam.com) (c)2019                                **
// **                                                                   **
// ** Updated: November 16, 2019                                        **
// **                                                                   **
// ** NOTE                                                              **
// ** ~~~~                                                              **
// ** For this to work on Raspberry Pi (Linux), you need to add the     **
// ** the following to the /boot/cmdline.txt...                         **
// **                                                                   **
// ** usbhid.quirks=0x2341:0x8036:0x040    <- for Leonardo              **
// ** usbhid.quirks=0x2341:0x8037:0x040    <- for Micro                 **
// **                                                                   **
// ***********************************************************************

// uncomment to use for serial terminal debugging rather than usb hid device
// this is so you can see the ascii values to verify
//#define SERIAL_DEBUG

#define J1UP 8
#define J1DOWN 7
#define J1LEFT 9
#define J1RIGHT 10
#define J1BUTTON 16

#define J2UP 3
#define J2DOWN 2
#define J2LEFT 4
#define J2RIGHT 5
#define J2BUTTON 6

#define J1Paddle0 2
#define J1Paddle1 3

#define J2Paddle0 0
#define J2Paddle1 1

Joystick_ _j1 = Joystick_(
    0x03,
    JOYSTICK_TYPE_JOYSTICK,
    1,
    0,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false  
  );
  
Joystick_ _j2 = Joystick_(
    0x06,
    JOYSTICK_TYPE_JOYSTICK,
    1,
    0,
    true,
    true,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false  
  );


byte _lastJ1X = 1; // 0 left, 1 center, 2 right
byte _lastJ1Y = 1; // 0 up, 1 center, 2 down
byte _lastJ1B = 0; // 0 button released, 1 button pressed

byte _lastJ2X = 1; // 0 left, 1 center, 2 right
byte _lastJ2Y = 1; // 0 up, 1 center, 2 down
byte _lastJ2B = 0; // 0 button released, 1 button pressed

void setup() {

#ifdef SERIAL_DEBUG
  Serial.begin(9600);
#else
  _j1.setXAxisRange(-127, 127);
  _j1.setYAxisRange(-127, 127);
  _j1.begin();
  
  _j2.setXAxisRange(-127, 127);
  _j2.setYAxisRange(-127, 127);
  _j2.begin();
#endif

  pinMode(J1UP, INPUT_PULLUP);
  pinMode(J1RIGHT, INPUT_PULLUP);
  pinMode(J1DOWN, INPUT_PULLUP);
  pinMode(J1LEFT, INPUT_PULLUP);
  pinMode(J1BUTTON, INPUT_PULLUP);

  pinMode(J2UP, INPUT_PULLUP);
  pinMode(J2RIGHT, INPUT_PULLUP);
  pinMode(J2DOWN, INPUT_PULLUP);
  pinMode(J2LEFT, INPUT_PULLUP);
  pinMode(J2BUTTON, INPUT_PULLUP);

  // TODO: adc for paddles
}

void loop() {

#ifdef SERIAL_DEBUG

  Serial.print(digitalRead(J1UP));
  Serial.print(digitalRead(J1RIGHT));
  Serial.print(digitalRead(J1DOWN));
  Serial.print(digitalRead(J1LEFT));
  Serial.print(digitalRead(J1BUTTON));

  Serial.print(digitalRead(J2UP));
  Serial.print(digitalRead(J2RIGHT));
  Serial.print(digitalRead(J2DOWN));
  Serial.print(digitalRead(J2LEFT));
  Serial.print(digitalRead(J2BUTTON));

  Serial.write("\r\n");

#else

  // ---------------------------------------------------------
  // JOYSTICK 1
  // ---------------------------------------------------------
  byte j1y;

  if (!digitalRead(J1UP))
    j1y = 0;
  else if (!digitalRead(J1DOWN))
    j1y = 2;
  else
    j1y = 1;

  if (j1y != _lastJ1Y) {

    if (j1y == 0)
      _j1.setYAxis(127);
    else if (j1y == 2)
      _j1.setYAxis(-127);
    else
      _j1.setYAxis(0);

    _lastJ1Y = j1y;
  }

  byte j1x;

  if (!digitalRead(J1LEFT))
    j1x = 0;
  else if (!digitalRead(J1RIGHT))
    j1x = 2;
  else
    j1x = 1;

  if (j1x != _lastJ1X) {

    if (j1x == 0)
      _j1.setXAxis(-127);
    else if (j1x == 2)
      _j1.setXAxis(127);
    else
      _j1.setXAxis(0);

    _lastJ1X = j1x;
  }

  byte j1b = !digitalRead(J1BUTTON);

  if (j1b != _lastJ1B) {

    _j1.setButton(0, j1b);

    _lastJ1B = j1b;
  }

  // ---------------------------------------------------------
  // JOYSTICK 2
  // ---------------------------------------------------------
  byte j2y;

  if (!digitalRead(J2UP))
    j2y = 0;
  else if (!digitalRead(J2DOWN))
    j2y = 2;
  else
    j2y = 1;

  if (j2y != _lastJ2Y) {

    if (j2y == 0)
      _j2.setYAxis(127);
    else if (j2y == 2)
      _j2.setYAxis(-127);
    else
      _j2.setYAxis(0);

    _lastJ2Y = j2y;
  }

  byte j2x;

  if (!digitalRead(J2LEFT))
    j2x = 0;
  else if (!digitalRead(J2RIGHT))
    j2x = 2;
  else
    j2x = 1;

  if (j2x != _lastJ2X) {

    if (j2x == 0)
      _j2.setXAxis(-127);
    else if (j2x == 2)
      _j2.setXAxis(127);
    else
      _j2.setXAxis(0);

    _lastJ2X = j2x;
  }

  byte j2b = !digitalRead(J2BUTTON);

  if (j2b != _lastJ2B) {

    _j2.setButton(0, j2b);

    _lastJ2B = j2b;
  }


#endif

  delay(25);
}
