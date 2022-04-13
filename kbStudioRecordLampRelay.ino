const String PROGRAMNAME = "kbStudio Record Lamp relay";
const String PROGRAMVERSION = "1.00";
const String PROGRAMDATE = "20220413";
const String PROGRAMAUTHOR = "(c) Kamil Baranski";
const String PROGRAMWWW = "kamilbaranski.com";

#include <SoftwareSerial.h>
#include <MIDI.h>
#include <ESP8266WiFi.h>  // we use it to disable WiFi ;)


/**
   debug
*/

// writes debug info on serial port (USB). 115200 bps!
const bool serialDebug = true;


/**
   relays
*/

// relay connected to GPIO12 (D6 on NodeMCU v3).
const uint8_t relayPin = 12;

// consts
const uint8_t STOP = 0;
const uint8_t RECORD = 1;
const String modes[2] = { "STOP", "RECORD" };


/**
   MIDI
*/

// MIDI In goes to GPIO13 (D7 on NodeMCU v3). We do not need to send anything, so TX=0.
#define MYPORT_TX 0
#define MYPORT_RX 13
SoftwareSerial myPort(MYPORT_RX, MYPORT_TX);

MIDI_CREATE_INSTANCE(SoftwareSerial, myPort, MIDI);

// which channel to listen to? MIDI_CHANNEL_OMNI listens to all.
const uint8_t CHANNEL = MIDI_CHANNEL_OMNI;


/***********************************************************************************
   let's begin!
*/

void setup() {
  disableWiFi();
  initSerial();
  delay(1000);  // to ensure COM is ready.
  debugMessage("\n\n" + PROGRAMNAME + " v. " + PROGRAMVERSION + " [" + PROGRAMDATE + "].\n(c) " + PROGRAMAUTHOR + "\n" + PROGRAMWWW + "\n");
  initMIDI();
  setPinModes();
  setRelay(STOP); // right after setPinModes(), cause else we have both relays opened.
}


void disableWiFi() {
  WiFi.mode(WIFI_OFF);
}

void initSerial() {
  // we don't use first serial port (USB) if we don't need to.
  if (serialDebug) {
    Serial.begin(115200);   // 9600 feels to slow.
  }
}

void initMIDI() {
  // software serial port. not sure if this line is correct/needed (MIDI standard is 31.25 kbit/s). But the program works ;)
  myPort.begin(38400, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
  if (!myPort) {
    // If the object did not initialize, then its configuration is invalid
    debugMessage("Invalid SoftwareSerial pin configuration, check config");
  }
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(CHANNEL);
}

void setPinModes() {
  pinMode(relayPin, OUTPUT);
}

void handleNoteOn(byte inChannel, byte inPitch, byte inVel) {
  debugMessage("+ CH=" + (String)inChannel + " N=" + (String)inPitch + " V=" + (String)inVel);
  if ((inChannel==1) && (inPitch==95)) {  // 95 is 5f = record.
    setRelay(RECORD);
  }
}

void handleNoteOff(byte inChannel, byte inPitch, byte inVel) {
  debugMessage("- CH=" + (String)inChannel + " N=" + (String)inPitch + " V=" + (String)inVel);
  if ((inChannel==1) && (inPitch==95)) {
    setRelay(STOP);
  }
}


void setRelay(uint8_t targetMode) {
  debugMessage("setRelays(" + (String)targetMode + "=" + modes[targetMode] + ");");
  if (targetMode == STOP) {
    digitalWrite(relayPin, !LOW);
  } else if (targetMode == RECORD) {
    digitalWrite(relayPin, !HIGH);
  }
}

void loop() {
  MIDI.read();
}

void debugMessage(String message) {
  if (serialDebug) {
    Serial.println(message);
  }
}
