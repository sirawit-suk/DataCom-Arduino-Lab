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
#define r_slope 100
 
void setup() {
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
int periodTime = 4000;

int prev = 0;
uint16_t text = 0;
bool check = false;
int max = 0;
int bitCount = 0;
bool lastAmp = true;

void loop() {
  now = micros();
  int temp = analogRead(A0); // read analog wave from A0
  
  // check period of input analog signal

  if (temp - prev > r_slope && check == false) { // RISING
    max = 0;
    check = true; // change check status is true
    if (lastAmp == true){
      lastAmp = false;
      past = now; // Reset Time
    }
  }

  if (temp > max) { // update max value  RISING
    max = temp; // Max amplitude
  }

  if (max - temp > r_slope){ // FALLING
    if (check == true){ // PEAK ALREADY
      cycleCount++;
      check = false;  // calculate input frequency
    }
  }
  
  if (now - past >= periodTime){
    lastAmp = true;
    
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
      text = 0;       // Reset text char
      bitCount = 0;   // Reset bitCount
    }      
  }
  
  prev = temp; // assign temp value to previous
}
