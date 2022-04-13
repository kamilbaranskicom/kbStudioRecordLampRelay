# kbStudioRecordLampRelay

&copy; Kamil Barański / [kamilbaranski.com](https://kamilbaranski.com/)

## DESCRIPTION
- Relay for the recording studio.
- Turns the relay on when record is on.

## HARDWARE
- ESP8266 / NodeMCU v3.
- Connect MIDI input to D7 (GPIO13). (Check [here](https://www.notesandvolts.com/2015/02/midi-and-arduino-build-midi-input.html) for the proper MIDI In schematics.)
- Connect relay control to D6 (GPIO12). Connect lamp to relay.
- Connect the MIDI input of the relay to the MIDI output of your DAW, set it as Mackie Control.

## SOFTWARE
- Arduino IDE with additional libraries:
  - [EspSoftwareSerial](https://www.arduino.cc/reference/en/libraries/espsoftwareserial/)
  - [MIDI Library](https://www.arduino.cc/reference/en/libraries/midi-library/)

<hr>
<sup><sub>Disclaimer: All the registered company names are used just to identify product lines. I'm not in any way connected with any of them.</sub></sup>