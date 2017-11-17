#pragma once

#include "Arduino.h"
#include "MorseCode.hpp"

#define MAX_MESSAGE_LENGTH 100

enum OutputState {
  ON = HIGH, OFF = LOW
};


struct LetterSender {
  LetterSender(){}
  LetterSender(char letter):
  mLetter(letter)
  {
    mCounter = 0;
    strcpy(mProsign, MorseCode::getProsign(letter));
    mProsignLength = strlen(mProsign);
  }
  
  char getNextCode() {
    return mProsign[mCounter++];
  }

  bool isFinished() const {
    return mCounter >= mProsignLength;
  }

protected:
  char mLetter;
  char mProsign[8];
  uint8_t mProsignLength;
  uint8_t mCounter = 0;
};

class Message {
  
public:
  Message(const char *message, float pauseAfter=0.f):
  mPauseAfter(static_cast<uint32_t>(pauseAfter * 1000)),
  mPin(2),
  mLetterSender(message[0]),
  mFinishedTransmission(false),
  mFinishedMessage(false),
  mLetterCounter(0)
  {
    strcpy(mMessage, message);
    mMessageLength = strlen(mMessage);
  }

  void update(uint32_t millisNow) {
    if (!mFinishedMessage && millisNow >= mNextChange) {
      transmitNext(millisNow);
    }
  }

  void startTransmission() {
    mLetterCounter = 0;
    mFinishedTransmission = mFinishedMessage = false;
    mLetterSender = LetterSender(mMessage[mLetterCounter++]);
  
    char c = mLetterSender.getNextCode();
    setOutput(ON);
    mNextChange = millis() + MorseCode::getLength(c);
  
  }

  void transmitNext(uint32_t millisNow) {
    if (mOutputState == ON) {
      setOutput(OFF);
      mNextChange = millisNow + MorseCode::getLength(MorseCode::AFTER_CODE);
    }
    else if (mLetterSender.isFinished()) {
      setOutput(OFF);
      mNextChange = millisNow + MorseCode::getLength(MorseCode::AFTER_LETTER);
  
      if(mLetterCounter < mMessageLength) {
        mLetterSender = LetterSender(mMessage[mLetterCounter++]);
      }
      else {
        if (!mFinishedTransmission) {
          mNextChange = millisNow + mPauseAfter;
          mFinishedTransmission = true;
        }
        else {
          mFinishedMessage = true;
        }
      }
    }
    else {
      char c = mLetterSender.getNextCode();
      setOutput(c != ' ' ? ON : OFF);
      mNextChange = millisNow + MorseCode::getLength(c);
    }
  }

  void setOutputPin(uint8_t pin) {
    mPin = pin;
    pinMode(pin, OUTPUT);
  }

  void setOutput(OutputState state) {
    digitalWrite(mPin, state);
    mOutputState = state;
  }

  bool isFinished() const {
    return mFinishedMessage;
  }

  virtual bool isLastMessage() const {
    return false;
  }

protected:
  char mMessage[MAX_MESSAGE_LENGTH];
  uint8_t mPin;

  uint16_t mLetterCounter, mMessageLength;
  bool mFinishedTransmission, mFinishedMessage;

  LetterSender mLetterSender;

  OutputState mOutputState;
  uint32_t mNextChange;
  
  uint32_t mPauseAfter;

  friend class Channel;
};

class EndMessage : public Message {
public:
  EndMessage(): Message("-", 0) {}

  bool isLastMessage() const override {
    return true;
  }
};
