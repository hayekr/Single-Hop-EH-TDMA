/*
Single Hop TDMA NODE (Energy Harvesting Simulation) (SEHTDMA) - Ohio Northern University
Author: Robert J. Hayek
Copyright (c) 2023, Robert J. Hayek. All rights reserved.
*/

#define LED 8


/* GLOBALS */
const int TIME_SLOT = 1000; // Time slot length in ms.
const int NUMBER_OF_NODES = 3;
const int NODE_NUMBER = 3;
const int ENERGY_HARVESTED_PROBABILITY = 40; // Percentage i.e 40%

/* COUNTERS AND FLAGS */
unsigned long count = 0;
unsigned long previous_action = 0;
unsigned long sync_time = 0;
bool sync_received = false;
bool wake_up_sync = false;
bool tx_allowed = false;
bool led_state = false;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

bool energyAvailable(int probability) {
  return random(0,100) < probability;
}

void nodeFSM(void) {
  unsigned long current = 0;

  static enum { SYNC_WAIT, READY_WAIT, TRANSMIT } state = SYNC_WAIT;
  
  switch (state) {
    case SYNC_WAIT:
      while(!sync_received) {
        if(Serial.available() > 0) {
          String incomingString = Serial.readStringUntil('\r');
          if(incomingString == "SYNC") {
            sync_received = true;
            tx_allowed = false;
            sync_time = millis();
          }
          if(isDigit(incomingString.charAt(0))) {
            state = READY_WAIT;
            count = incomingString.toInt();
            wake_up_sync = true;
            sync_received = true;
            sync_time = millis();
            break;
          }
          Serial.flush();
        }
      }
      while(millis() - previous_action <= 2000 && !wake_up_sync); // 2 Delay, arbitrary, for my sake when timeslot is short
      state = READY_WAIT;
      Serial.flush();
      previous_action = millis();
      previous_action -= previous_action - sync_time;
      break;

    case READY_WAIT:
      current = millis();
      if(current - previous_action >= TIME_SLOT) {
          previous_action += TIME_SLOT;
          count++;
          if(count % NUMBER_OF_NODES == NODE_NUMBER % NUMBER_OF_NODES) {
            state = TRANSMIT;
            tx_allowed = true;
            break;
          }
          else state = READY_WAIT;
      }
      if(Serial.available() > 0 && !tx_allowed && sync_received) {
        if(Serial.read() == 'R') {
          Serial.flush();
          delay(TIME_SLOT * NODE_NUMBER);
          Serial.println('A');
          sync_received = false;
          wake_up_sync = false;
          count = 0;
          state = SYNC_WAIT;
          break;
        }
      }
      break;

    case TRANSMIT:
      if(energyAvailable(ENERGY_HARVESTED_PROBABILITY)) {
        Serial.println(NODE_NUMBER);
        led_state = !led_state;
        digitalWrite(LED, led_state);
        state = READY_WAIT;
        tx_allowed = false;
      }
      else {
        count = 0;
        previous_action = 0;
        sync_time = 0;
        sync_received = false;
        wake_up_sync = false;
        tx_allowed = false;
        led_state = false;
        state = SYNC_WAIT;
      }
      break;

    default:
      state = SYNC_WAIT;
      break;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() {
  nodeFSM();
}