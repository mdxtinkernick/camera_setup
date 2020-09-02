// Trellis M4 MIDI Keypad CC
// sends 32 notes over USB MIDI


#include <Adafruit_NeoTrellisM4.h>

#define MIDI_CHANNEL     0  // default channel # is 0
// Set the value of first note, C is a good choice. Lowest C is 0.
// 36 is a good default. 48 is a high range. Set to 24 for a bass machine.
#define FIRST_MIDI_NOTE 1

uint32_t const positionColor = 0xFF4500;
uint32_t const cameraColor = 0x0000FF;
uint32_t const selectedColor = 0x00FF00;
uint32_t const turnOffColor = 0xFF0000;
uint32_t const turnOnColor = 0x4B0082;
uint32_t const switchColor = 0x555500;



int selectedPositionKey = -1;
int selectedCameraKey = -1;
int selectedSwitchKey = -1;

uint32_t button_colors[32] = {
  positionColor, 0x000000, 0x000000, 0x000000, 0x000000, positionColor, 0x000000, turnOnColor,
  0x000000, cameraColor, cameraColor, cameraColor, cameraColor, 0x000000, 0x000000, 0x000000,
  positionColor, 0x000000, 0x000000, 0x000000, 0x000000, positionColor, 0x000000, 0x000000,
  0x000000, switchColor, switchColor, switchColor, switchColor, 0x000000, 0x000000, turnOffColor
};



Adafruit_NeoTrellisM4 trellis = Adafruit_NeoTrellisM4();

void setup() {
  Serial.begin(115200);
  //while (!Serial);
  Serial.println("MIDI keypad");

  trellis.begin();
  trellis.setBrightness(80);

  // USB MIDI messages sent over the micro B USB port
  Serial.println("Enabling MIDI on USB");
  trellis.enableUSBMIDI(true);
  trellis.setUSBMIDIchannel(MIDI_CHANNEL);
  for (int light = 0; light < 32; light++) {
    trellis.setPixelColor(light, button_colors[light]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  trellis.tick();

  while (trellis.available()) {
    keypadEvent e = trellis.read();
    int key = e.bit.KEY;
    Serial.print("Keypad key: ");    Serial.println(key);
    Serial.print("MIDI note: ");     Serial.println(FIRST_MIDI_NOTE + key);

    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      Serial.println(" pressed\n");
      if (button_colors[key] == switchColor) {
        trellis.noteOn(FIRST_MIDI_NOTE + key, 64);
        if (selectedSwitchKey > -1) {
          trellis.setPixelColor(selectedSwitchKey, switchColor);
        }
        trellis.setPixelColor(key, selectedColor);
        selectedSwitchKey = key;
      }
      if (button_colors[key] == positionColor) {
        trellis.noteOn(FIRST_MIDI_NOTE + key, 64);
        if (selectedPositionKey > -1) {
          trellis.setPixelColor(selectedPositionKey, positionColor);
        }
        trellis.setPixelColor(key, selectedColor);
        selectedPositionKey = key;
      }
      if (button_colors[key] == cameraColor) {
        trellis.noteOn(FIRST_MIDI_NOTE + key, 64);
        if (selectedCameraKey > -1) {
          trellis.setPixelColor(selectedCameraKey, cameraColor);
        }
        trellis.setPixelColor(key, selectedColor);
        selectedCameraKey = key;
      }
      if (button_colors[key] == turnOffColor) {
        trellis.noteOn(FIRST_MIDI_NOTE + key, 64);
      }
      if (button_colors[key] == turnOnColor) {
        trellis.noteOn(FIRST_MIDI_NOTE + key, 64);
      }
    }
  }
  trellis.sendMIDI(); // send any pending MIDI messages

  delay(10);
}
