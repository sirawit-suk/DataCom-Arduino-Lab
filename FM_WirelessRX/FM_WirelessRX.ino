// TEA5767 Example

#include <Wire.h>
#include <TEA5767Radio.h>

TEA5767Radio radio = TEA5767Radio();

#define baudrate 250
int cyc[4] = {500/baudrate, 750/baudrate, 1000/baudrate, 1250/baudrate}; //500,750,1000,1250
//int bits[4] = {0,1,2,3};
// cbi this for increase analogRead Speed 
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// amplitude diff. for detecting rising or falling signal
#define r_slope 130// r > 110 130 150 < r //
 
void setup() {
  Wire.begin();
  radio.setFrequency(107.5); // pick your own frequency

  Serial.begin(115200);

  sbi(ADCSRA,ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;

  Serial.println("Receive Ready : ");
  Serial.flush();
}


int cycleCount = 0;
unsigned long int now = 0;
unsigned long int past = 0;
int periodTime = 3700; // 3850

int prev = 450; // middle of voltage
uint16_t text = 0;
bool check = false;
int min = 800; // min mode
int bitCount = 0;
bool lastAmp = true;

void loop() {
  now = micros();
  int temp = analogRead(A2); // read analog wave from A0
  
  // check period of input analog signal
  //if (temp > 600) Serial.println(temp);
  if (prev - temp > r_slope && check == false) { // RISING ## FALLING
    min = 800;
    check = true; // change check status is true
    
    if (lastAmp == true){
      lastAmp = false;
      past = now; // Reset Time
    }
  }
  if (temp < min) { // update max value  RISING ## FALLING
    min = temp; // Max amplitude ## MIN!
  }
  
  if (temp - min > r_slope){ // FALLING ## RISING!!!
    if (check == true){ // PEAK ALREADY
      cycleCount++;
      check = false;  // calculate input frequency
    }
  }
  if (now - past >= periodTime){
    lastAmp = true;
    /*
    if (cycleCount > 0){
      Serial.print(bitCount);
      Serial.print("  ");
      Serial.println(cycleCount);
      cycleCount = 0;
      bitCount++;
    }
    */
    for(int i = 0; i < 4; i++){
      if (cyc[i] == cycleCount){  // decode data bits from detected input frequency
         //Serial.print(cycleCount);
         int t = (i & 3) << (bitCount*2);  //2 bits
         text |= t;
         bitCount++;
         cycleCount = 0;   // Reset cycleCount
      }
    }

    if (bitCount == 4){
      Serial.print(char(text));   // show read data bits 
      //Serial.flush();
      text = 0;       // Reset text char
      bitCount = 0;   // Reset bitCount
    }      
  }
  
  prev = temp; // assign temp value to previous
}
