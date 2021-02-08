#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
int delay0;

#define degree_Size 32
#define defaultFreq 1700  // dac speed
#define freq0 7000     // sine wave frequency

float zeta[degree_Size];
float S[degree_Size]; // store sine value
uint16_t S_DAC[degree_Size]; // store Duty Cycle of PWM Signal

void setup() {
  for(int i = 0; i < degree_Size;i++){
      i == 0 ? zeta[i] = 0 : zeta[i] += zeta[i-1]+(360.0/degree_Size);
  }
  
  Serial.begin(115200); // set vaudrate serial
  //dac.begin(0x62);      // set to default for MCP4725A1
  dac.begin(0x64);
  //dac.begin(0x60);
  delay0 = (1000000/freq0 - 1000000/defaultFreq)/4;
  Serial.print("delay0 is ");
  Serial.println(delay0);

  for(int i = 0 ; i < degree_Size ; i++ ){
    S[i] = sin(zeta[i]/180 * PI);
  }

  for (int i = 0 ; i < degree_Size ; i++){
    S_DAC[i] = map(S[i]*100,-100,100,0,4095);
    Serial.print("S[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.print(S[i]);
    
    Serial.print(" - S_DAC[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(S_DAC[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0 ; i < degree_Size ; i++){
    Serial.println(S_DAC[i]);
    dac.setVoltage(S_DAC[i], false);
    delayMicroseconds(delay0);
  }
  
}
