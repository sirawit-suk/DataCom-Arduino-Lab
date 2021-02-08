#include <SoftwareSerial.h>

SoftwareSerial popeyeza12(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  popeyeza12.begin(57600);
  delay(200);  //break for available to read...
}

void loop() {
  delay(100);  //break for available to read...

  //MY COMPUTER 
  if(Serial.available()){             //Only Once
    String text1 = "";
    while(Serial.available()){
      char one = Serial.read();
      popeyeza12.write(one);          //READ ME ->SEND
      text1 += one;
      delay(5);  //break time
    }
    Serial.print("Me: ");
    Serial.println(text1);               
  }

  //OTHER 
  if (popeyeza12.available()){           //TO ME <- RECEIVE 
    String text2 = "";
    while(popeyeza12.available()){
      char one = popeyeza12.read();
      text2 += one;
      delay(5);  //break time
    }
    Serial.print("B: ");
    Serial.println(text2);
  }  

}
