/*
  _____  _       _                      _                      _   _  __             _____            _             _ _           
 |  __ \(_)     (_)                    | |                    | | | |/ /            / ____|          | |           | | |          
 | |  | |_  __ _ _ ___ _ __   __ _ _ __| | __   ___  ___ _   _| | | ' / ___ _   _  | |     ___  _ __ | |_ _ __ ___ | | | ___ _ __ 
 | |  | | |/ _` | / __| '_ \ / _` | '__| |/ /  / _ \/ __| | | | | |  < / _ \ | | | | |    / _ \| '_ \| __| '__/ _ \| | |/ _ \ '__|
 | |__| | | (_| | \__ \ |_) | (_| | |  |   <  | (_) \__ \ |_| |_| | . \  __/ |_| | | |___| (_) | | | | |_| | | (_) | | |  __/ |   
 |_____/|_|\__, |_|___/ .__/ \__,_|_|  |_|\_\  \___/|___/\__,_(_) |_|\_\___|\__, |  \_____\___/|_| |_|\__|_|  \___/|_|_|\___|_|   
            __/ |     | |                                                    __/ |                                                
           |___/      |_|                                                   |___/      
           */
// Original Author: Daniel Jose Viana, danjovic@hotmail.com
//
// Version 0.6 - March, 2016 - Basic Release

//Edited for osu! by kuwoh
//note: removed some uneccesarry stuff only Z and X keys are present (add more if you need to)


////////////////////////////////////////////////////////////////////////////////
//    ___       __ _      _ _   _
//   |   \ ___ / _(_)_ _ (_) |_(_)___ _ _  ___
//   | |) / -_)  _| | ' \| |  _| / _ \ ' \(_-<
//   |___/\___|_| |_|_||_|_|\__|_\___/_||_/__/
//
#define _Pin_Left_switch    0
#define _Pin_Right_switch   2

#define _Key_Left_switch KEY_Z   // LEFT CLICK
#define _Key_Right_switch KEY_X  // RIGHT "


////////////////////////////////////////////////////////////////////////////////
//    _    _ _                 _
//   | |  (_) |__ _ _ __ _ _ _(_)___ ___
//   | |__| | '_ \ '_/ _` | '_| / -_|_-<
//   |____|_|_.__/_| \__,_|_| |_\___/__/
//
#include "DigiKeyboard.h"


////////////////////////////////////////////////////////////////////////////////
//   __   __        _      _    _
//   \ \ / /_ _ _ _(_)__ _| |__| |___ ___
//    \ V / _` | '_| / _` | '_ \ / -_|_-<
//     \_/\__,_|_| |_\__,_|_.__/_\___/__/
//

uint8_t lastkeys = 0;
uint8_t keys;

////////////////////////////////////////////////////////////////////////////////
//    ___             _   _
//   | __|  _ _ _  __| |_(_)___ _ _  ___
//   | _| || | ' \/ _|  _| / _ \ ' \(_-<
//   |_| \_,_|_||_\__|\__|_\___/_||_/__/
//




////////////////////////////////////////////////////////////////////////////////
//    ___ ___ _____ _   _ ___
//   / __| __|_   _| | | | _ \
//   \__ \ _|  | | | |_| |  _/
//   |___/___| |_|  \___/|_|
//
void setup() {

  // Set pullups for pedal switches and define them as inputs
  digitalWrite(_Pin_Left_switch, HIGH);
  pinMode(_Pin_Left_switch, INPUT);

  digitalWrite(_Pin_Right_switch, HIGH);
  pinMode(_Pin_Right_switch, INPUT);

  // This is needed before start sending keys
  DigiKeyboard.sendKeyStroke(0);

}



////////////////////////////////////////////////////////////////////////////////
//    __  __      _        _
//   |  \/  |__ _(_)_ _   | |   ___  ___ _ __
//   | |\/| / _` | | ' \  | |__/ _ \/ _ \ '_ \
//   |_|  |_\__,_|_|_||_| |____\___/\___/ .__/
//                                      |_|
void loop() {

  uint8_t i; // index for keystrokes

  // Sample footkeys. We have 3 keys
  footkeys = 0;

  if ((digitalRead(_Pin_Left_switch) == 0))   keys |= (1 << _Pin_Left_switch);
  if ((digitalRead(_Pin_Right_switch) == 0))  keys |= (1 << _Pin_Right_switch);

  // if switches changed state send new report to host
  if (keys != lastkeys) {                // on change
    lastkeys = keys;                     // update key state

    // Sync with the next interrupt
    while (!usbInterruptIsReady()) {
      usbPoll();
      _delay_ms(5);
    }

    // clear buffer by default
    memset( DigiKeyboard.reportBuffer , 0, sizeof( DigiKeyboard.reportBuffer));

    // Now set active keys in report
    if (footkeys) {
      i = 1; // point to first key from report
      if (keys & (1 << _Pin_Left_switch))   DigiKeyboard.reportBuffer[i++] = _Key_Left_switch;
      if (keys & (1 << _Pin_Right_switch))  DigiKeyboard.reportBuffer[i++] = _Key_Right_switch;
    }

    // Now set to send the keystrokes in the next USB interrupt
    usbSetInterrupt( DigiKeyboard.reportBuffer, sizeof(DigiKeyboard.reportBuffer));
    while (!usbInterruptIsReady()) {
      usbPoll();
      _delay_ms(5);
    }
  }

  // cycle each 1ms
  DigiKeyboard.delay(1);
}

////////////////////////////////////////////////////////////////////////////////
// Ascii Titles by Patrick Gillespie http://patorjk.com/software/taag



