#define defaultFreq 1700 //DAC speed (Hz)
//freq0 : frequency of carrier sine wave (Hz)
#define freq0 1700
//A[0]-A[3] : ASK Amplitude (0,5] (V)
const float A[4] = {1, 2, 3, 4};
int delay0;

// amin/amax : Amplitude in digital 10bit
#define a0min 1.13*1023/5 // a0min <= a0(1v) <= a0max  1.13 - 1.38
#define a0max 1.38*1023/5
#define a1min 2.26*1023/5 // a1min <= a1(2v) <= a1max  2.26 - 2.70
#define a1max 2.70*1023/5
#define a2min 3.45*1023/5 // a2min <= a2(3v) <= a2max  3.45 - 3.89 
#define a2max 3.89*1023/5
#define a3min 4.71*1023/5 // a3min <= a3(4) <= a3max   4.71 - 5.00
#define a3max 5.00*1023/5
#define r_slope 100

int sum = 0;
int max = 0;
int prev = 0;
int check = false;
int count = 0;

int charCount = 0;

void setup() {
  // set serial baudrate the same as in TX
  Serial.begin(115200);
  Serial.println("Receive Ready!");
}
int text;
void loop() {
  int temp = analogRead(A0); // read signal from analog pin ALWAYS READ!!

  if (temp - prev > r_slope && check == false) {
    max = 0;
    check = true; // change check status is true
  }

  if (temp > max) { // update max value
    //Serial.println(temp);
    max = temp; // Max amplitude
  }

  if (max - temp > r_slope) { // check for falling signal
    //Serial.print("Max : ");
    //Serial.print(max);
    //Serial.print(" temp : ");
    //Serial.println(temp);
    if (check == true) {
      int a;
      if (a0min < max && max < a0max) {
        a = 0;
        count++;
        //Serial.print(" 00 ");
      }
      else if (a1min < max && max < a1max) {
        a = 1;
        count++;
        //Serial.print(" 01 ");
      }
      else if (a2min < max && max < a2max) {
        a = 2;
        count++;
        //Serial.print(" 10 ");
      }
      else if (a3min < max && max < a3max) {
        a = 3;
        count++;
        //Serial.print(" 11 ");
      }
      if (count == 5) { // 5 cycles
        int t = (a & 3) << (charCount * 2); // LAST 2 BIT
        text |= t;    // sum (OR) bit
        charCount++; //


        if (charCount == 4) {
          Serial.print(char(text));
          charCount = 0; // Reset = 0
          text = 0;      // Reset = 0x00
        }
        count = 0;
      }
    }

    check = false; // change check status is false
  }
  prev = temp; // assign temp value to previous
}
