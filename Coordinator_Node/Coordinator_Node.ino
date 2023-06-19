/*
Single Hop Energy Harvesting TDMA COORDINATOR (Energy Harvesting Simulation) (SEHTDMA) - Ohio Northern University
Author: Robert J. Hayek
Advisor: Dr. Ahmed Ammar
Copyright (c) 2023, Robert J. Hayek. All rights reserved.
*/
#include <math.h>

/* GLOBALS */
const int TIME_SLOT = 56;
const int NUMBER_OF_NODES = 3; 
const int TIMEOUT = 10000;
const int THRESHOLD = 200;

/* FLAGS AND STUFF*/
String last_message;
String current_message;
unsigned long previous_resync = 0;
unsigned long current_timestamp = 0;
unsigned long last_timestamp = 0;
unsigned long start_recv = 0;

int packet_count = 0;
int last_packet_count = 0;
int recieved_acks = 0;
int resync_sent_count = 0;

bool packet_recieved = false;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void coordinatorFSM(void) {
  static enum { SYNC, RECV, PSYNC } state = SYNC;
  switch(state) {
    case SYNC:
      Serial.println("SYNC");
      last_message = "";
      current_message = "";
      Serial.flush();
      state = RECV;
      start_recv = millis();
      break;
    
    case RECV:
      if(Serial.available() > 0) {
        last_message = current_message;
        current_message = Serial.readStringUntil('\n');
        last_timestamp = current_timestamp;
        current_timestamp = millis();
        if(current_message.toInt() == 0) {
          packet_recieved = false;
        }
        else {
          packet_recieved = true;
        }
      }
      if(packet_recieved) {
        packet_recieved = false;
        last_packet_count = packet_count;
        packet_count++;
        if(packet_count % NUMBER_OF_NODES == current_message.toInt() % NUMBER_OF_NODES) {
          start_recv = millis();
          state = RECV;
        }
        else {
          float t_diff = current_timestamp - last_timestamp;
          t_diff = round(t_diff / 1000);
          if(t_diff*1000 >= 2*(TIME_SLOT) - THRESHOLD) {
            packet_count = current_message.toInt();
            start_recv = millis();
            state = RECV;
          }
          else {
            state = PSYNC;
          }
        }
      }
      if(millis() - start_recv > TIMEOUT && !packet_recieved) {
        Serial.println("RESET");
        Serial.flush();
        delay(2000);
        resetFunc();  //call reset
      }
      Serial.flush();
      break;

    case PSYNC:
        previous_resync = millis();
        while(recieved_acks == 0) {
          if(millis() - previous_resync >= TIME_SLOT) {
          Serial.println('R');
          Serial.flush();
          resync_sent_count++;
          previous_resync = millis();
          }
          if(Serial.available() > 0) {
            if(Serial.read() == 'A') {
              Serial.flush();
              recieved_acks++;
            }
          }
        if(resync_sent_count > NUMBER_OF_NODES + 1) break;
        }
        delay(10000); // Arbitrary delay to have a pause for my sake

        // Reset everything
        recieved_acks = 0;
        previous_resync = 0;
        current_timestamp = 0;
        last_timestamp = 0;
        packet_count = 0;
        last_packet_count = 0;
        start_recv = 0;
        resync_sent_count = 0;
        packet_recieved = false;
        state = SYNC;
        Serial.flush();
      break;

    default:
      state = SYNC;
      break;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(TIME_SLOT);
}

void loop() {
  coordinatorFSM();
}
