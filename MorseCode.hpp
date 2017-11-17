#pragma once

#include "Arduino.h"

#define PROSIGN_CHAR_OFFSET 32

class MorseCode {
public:

  enum {
    AFTER_CODE = 'c',
    AFTER_LETTER = 'l',
  };
  
  static const char* prosigns[];

  static const char* getProsign(char c) {
    if (c >= 'a' && c <= 'z') {
      c-= 32; // make uppercase if lowercase
    }
    return MorseCode::prosigns[c - PROSIGN_CHAR_OFFSET];
  }
  
  static uint16_t getLength(char signal) {
    switch (signal) {
    case '.':
      return MorseCode::timeStep;
    case '-':
      return MorseCode::timeStep * 3;
    case AFTER_CODE:
      return MorseCode::timeStep;
    case AFTER_LETTER:
      return MorseCode::timeStep * 3;
    case ' ':
      // spaces are treated as normal characters so are
      // and the AFTER_CODE off time from the previous letter
      return MorseCode::timeStep * 6;
    }
  }

  static uint16_t timeStep; // ms
};
