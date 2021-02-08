#include <Wire.h>
#include <Adafruit_MCP4725.h>

#define defaultFreq 1700 // dac speed (Hz)
#define baudrate 250

int S_DAC[4] = {2000,4000,2000,0};  //S_DAC : Amplitude (12bit) of sine wave at 0,90,180,270
Adafruit_MCP4725 dac;

int delaySample[4] = {};
int freq[4] = {500,750,1000,1250};

char input[30] = {}; //buffer input

void setup() {
  Serial.begin(115200);
  dac.begin(0x64); // set to default
  for(int i = 0; i < 4; i++){
    delaySample[i] = (1000000 / freq[i] - 1000000 / defaultFreq) / 4; //sampling period for FSK 500,750,1000,1250 Hz
  }

  Serial.println("Enter Text:");
  Serial.flush(); // for clear buffer serial
}

void loop() {
  if (Serial.available() > 0) { // for get input

    int count = 0;

    for (int i = 0 ; Serial.available() > 0; i++) {
      input[i] = Serial.read();
      count++;
    }

    for (int i = 0; i < count; i++) { // Every charactors

      for (int k = 0; k < 4; k++) { //4 Group of 2 bits
        int temp = input[i] & 3; // 00, 01, 10, 11 only 2 bit right
        int useDelay = 0;
        int cyc = 0;

        for (int j = 0; j < 4 ; j++){   // check Encoding Rules (2 bits, 4 samples)
          if (temp == j){       
            cyc = freq[j] / baudrate;   // make cycles
            useDelay = delaySample[j];  // make delay
          }
        }

        // SEND !!!
        for (int cycle = 0; cycle < cyc; cycle++) {
          for (int sample = 0; sample < 4; sample++) {
            dac.setVoltage(S_DAC[sample], false); 
            delayMicroseconds(useDelay);
          }
        }
        
        input[i] >>= 2; // Shift 2 bits to right
      }
      // End 1 Char
    }
    // End every Char
    
    dac.setVoltage(0, false); // default
  }
}
