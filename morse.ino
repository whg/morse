#include "Message.hpp"
#include "Channel.hpp"

Message* channel_1_messages [] = {
  new Message("I am here, where are you?", 4),
  new Message("Distant shapes on the horizon"),
  new Message("Sentinels move forward", 1),
  new Message("You are forewarned", 1),
  new EndMessage()
};

Message* channel_2_messages [] = {
  new Message("Rest now", 4),
  new Message("We are not lost"),
  new Message("Really not sure if this Morse is correct, so please check!"),
  new EndMessage()
};

// this is the time for 1 dot
uint16_t MorseCode::timeStep = 100; // in milliseconds

///////////////////////////////////////////////////////////////////////

#define NUM_CHANNELS 2
Channel channels[NUM_CHANNELS] = {
  Channel(channel_1_messages, 7),
  Channel(channel_2_messages, 6),  
};


void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < NUM_CHANNELS; i++) {
    channels[i].start();
  }

}

void loop() {

  uint32_t t = millis();
  for (uint8_t i = 0; i < NUM_CHANNELS; i++) {
    Channel &channel = channels[i];
    if (channel.update(t)) {
      channel.start();
    }
  }

  delay(5);
}

