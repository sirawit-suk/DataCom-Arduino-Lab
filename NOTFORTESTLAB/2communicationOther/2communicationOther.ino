#include <SoftwareSerial.h>

SoftwareSerial popeyeza24(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  popeyeza24.begin(57600);
  delay(200);  //break for available to read...
}

void loop() {
  delay(100);  //break for available to read...
  //MY COMPUTER 
  if(Serial.available()){             //Only Once
    String text1 = "";
    while(Serial.available()){
      
      char one = Serial.read();
      popeyeza24.write(one);          //READ ME ->SEND
      text1 += one;
      delay(5);  //break time
    }
    Serial.print("Me: ");
    Serial.println(text1);               
  }
    

  //OTHER   
  if (popeyeza24.available()){
    String text2 = "";
    while(popeyeza24.available()){
      char one = popeyeza24.read();
      text2 += one;
      delay(5);  //break time
    }
    Serial.print("A: ");
    Serial.println(text2);
  }
    

}
