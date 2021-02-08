#define pwmPin 3
#define degree_Size 8


float zeta[degree_Size];
float S[degree_Size]; // store sine value
uint16_t pwmDuty[degree_Size];


void setup() {
    for(int i = 0; i < degree_Size;i++){
      i == 0 ? zeta[i] = 0 : zeta[i] += zeta[i-1]+(360.0/degree_Size);
    }

    Serial.begin(115200);
    pinMode(pwmPin,OUTPUT);

    //presume that f = 1
    //presume that t = 1
    
    for(int i = 0; i<degree_Size; i++){
      //S[i] = sin(2*PI*f*t);  formula
      S[i] = sin(zeta[i]/180 * PI); // 0 -> 0 // 90 -> PI/2  // 180 -> PI // 270 -> PI*3/2 // 360 -> PI*2 
      Serial.println(S[i]);
    }

    for(int i = 0; i<degree_Size; i++){
      pwmDuty[i] = map(S[i]*100,-100,100,0,4095);
      //Serial.print(i);
      Serial.print(" ");
      Serial.print(pwmDuty[i]);
    }
    
}

void loop() {

  for(int i = 0; i<degree_Size; i++){
    analogWrite(pwmPin, pwmDuty[i]);
    //Serial.println(pwmDuty[i]);
    delay(4);
  }
  
}
