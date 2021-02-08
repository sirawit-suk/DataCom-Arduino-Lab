#include <SoftwareSerial.h>

SoftwareSerial popeyeza24(10, 11); // RX, TX

char myID;

void setup() {
  delay(200);  //break for available to read...
  Serial.begin(9600);
  popeyeza24.begin(57600);
  Serial.println("Enter ID:");
  while(!Serial.available());     //Break..wait for typing...
  myID = Serial.read();
  Serial.print("Your ID : ");
  Serial.println(myID);
  
  delay(200);  //break for available to read...
}

void loop() {
  delay(100);  //break for available to read...

  //MY COMPUTER 
  if(Serial.available()){             //Only Once
    String text1 = "";
    String frame = "";
    while(Serial.available()){
      char one = Serial.read();
      
      text1 += one;
      delay(5);  //break time
    }

    frame += text1[0];
    frame += myID;
    for(int i = 2; i<text1.length() ; i++){
      frame += text1[i];
    }
    for(int i = 0; i<frame.length() ; i++){
      popeyeza24.write(frame[i]);          //READ ME ->SEND
    }

    Serial.print("Me:");
    for(int i = 2; i<text1.length() ; i++){
      Serial.print(text1[i]);  
    }
    Serial.println("");
                 
  }

  //OTHER 
  if (popeyeza24.available()){           //TO ME <- RECEIVE 
    String text2 = "";
    while(popeyeza24.available()){
      char one = popeyeza24.read();
      text2 += one;
      delay(5);  //break time
    }
    if(text2[0] == myID){
      Serial.print(text2[1]);
      Serial.print(":");
      for(int i = 2; i<text2.length() ; i++){
        Serial.print(text2[i]);
      }  
      Serial.print("\n");
    }else{
      for(int i = 0; i<text2.length() ; i++){
        popeyeza24.write(text2[i]);          //READ ME ->SEND
      }
    }

  }  

}
