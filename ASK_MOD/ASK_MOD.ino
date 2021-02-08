#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700  //DAC speed (Hz)

//freq0 : frequency of carrier sine wave (Hz)
#define freq0 500

//A[0] - A[3] : ASK Amplitude (0,5] (V)
const float A[4] = {1, 2, 3, 4}; // DEFINEE!!!

//S_DAC: Amplitue(12bit) of sine wave at 0, 90, 180, 270
const float zeta[4] = {0, 90, 180, 270};

float S[4];
uint16_t S_DAC[4];

int delay0;
char inData[20]; //Allocate some space for the string

void setup( ) {
  Serial.begin(115200);
  for(int i = 0 ; i < 4 ; i++ ){
    S[i] = sin(zeta[i]/180 * PI);
  }

  for (int i = 0 ; i < 4 ; i++){
    S_DAC[i] = map(S[i]*100,-100,100,0,4095); // STORE S_DAC!!!!
    Serial.print("S[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.print(S[i]);
    
    Serial.print(" - S_DAC[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(S_DAC[i]);
  }
  
  /* set buadrate serial is 115200 */
  dac.begin(0x64); // set to default for us use regis 0x64
  delay0 = (1000000 / freq0 - 1000000 / defaultFreq) / 4; // same formula
  // delay for sampling period of sine
  // (Tsine – delayfrom DAC processing speed)
  Serial.flush(); // for clear buffer serial
}

void loop( ) {
  
  if (Serial.available() > 0) { // if have serial input
    
    // input message 
    for (int i = 0; i == 0 || inData[i - 1] != 0xFFFFFFFF; i++) { // first time or before last is -1
      inData[i] = Serial.read(); // Read a character // if read nothing.. will be 0xFFFFFFFF or -1
    }
    
    // cycle send data 8 bits
    for (int i = 0; inData[i] != 0xFFFFFFFF; i++ ) { // check if read -1
      // LSB to MSB 8 bits
      
      for (int k = 0; k < 8; k += 2) { // 4 group of 2 bits(LSB to MSB)
        int temp = inData[i] & 3; // 00, 01, 10, 11  only 2 bit right (LSB)
        
        for (int cycle = 0; cycle < 5; cycle++ ) { // send 5 cycle/baud
          for (int sample = 0; sample < 4; sample++ ) { // gen 4 sample/cycle

            dac.setVoltage(S_DAC[sample % 4]*A[temp] / 4, false);
            delayMicroseconds(delay0); // sampling delay
          }
        }
        inData[i] >>= 2;  //Shift 2 bit to the right !!!
      }
    }
    dac.setVoltage(0, false); // default 0 V for don't send
  }
}
