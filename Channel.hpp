#pragma once

#include "Arduino.h"

class Channel {
public:
  Channel(Message **messages, uint8_t pin):
  mMessages(messages)
  {
    for (uint8_t i = 0;; i++) {
      if (messages[i]->isLastMessage()) {
      break;
      }
      messages[i]->setOutputPin(pin);
    }
  
    start();
  }

  void start() {
    mCurrentMessageIndex = 0;
    mMessages[mCurrentMessageIndex]->startTransmission();
  }

  bool update(uint32_t millisNow) {

    Message *currentMessage = mMessages[mCurrentMessageIndex];
    currentMessage->update(millisNow);

    if (currentMessage->isFinished()) {
      currentMessage = mMessages[++mCurrentMessageIndex];
    
      if (currentMessage->isLastMessage()) {
        mCurrentMessageIndex = 0;
        return true;
      }
      else {
        currentMessage->startTransmission();
      }

    }
    return false;
  }

protected:
  Message **mMessages;
  uint8_t mCurrentMessageIndex;
  Message *mCurrentMessage;
};
