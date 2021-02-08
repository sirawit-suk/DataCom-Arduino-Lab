#include <SoftwareSerial.h>

SoftwareSerial popeyeza12(10,11);

char id;
char receiver;
char flag = '$';
char escape = '\\';

void setup() {
  Serial.begin(115200);
  popeyeza12.begin(57600);

  //Serial.println(makeSum("Forouzan"));
  
  //Enter ID:
  Serial.println("Enter ID : ");
  //Waiting...
  while(!Serial.available());
  id = Serial.read();
  //Print ID
  Serial.print("My ID is : ");
  Serial.println(id);

}

void loop() {
  
  sendTask();
  receiveTask();

}

void sendTask(){
  char myString[] = "";
  
  //Send to Others!!!
  if(Serial.available()){
    receiver = Serial.read();
    Serial.print("Receiver is : ");
    Serial.println(receiver);
    
    while(!Serial.available());
    String sendText = Serial.readStringUntil('\n');
    String tempText = makeFrame(receiver,id,sendText);
    for (int i=0; tempText[i] != '\0'; i++){
      popeyeza12.write(tempText[i]);
      delay(10);
      flushRx();
    }
  }
}

void receiveTask(){
  //Recive From Others!!!
  if(popeyeza12.available()){
    String rep = popeyeza12.readStringUntil('\n');
    receiveFrame(rep);
  }
}

String makeFrame(char receiver,char sender, String dataword){  // Send to Receiver w/data
  String codeword = dataword + makeSum(dataword);
  String sendText = "";
  
  // insert double flag and double escape
  for (int i = 0; codeword[i] != '\0'; i++){       // variable size
      if(codeword[i] == flag || codeword[i] == escape){
        sendText += escape;
        sendText += codeword[i];
      }else {
        sendText += codeword[i];
      }
  }
  String frameText = "";
  frameText += flag;
  frameText += receiver;
  frameText += sender;
  frameText += sendText;
  frameText += flag;
  Serial.println(frameText);
  return frameText;
  
}

String makeSum(String data){
  char dataArray[data.length()];
  data.toCharArray(dataArray, data.length());

  int sum = 0;
  for(int i = 0; dataArray[i] != '\0'; i++){    //Plus every char to int...
    sum += int(dataArray[i]);
  }

  return String(sum);
}

void receiveFrame(String frame){
  int num = 0;
  String data = "";
  int ack = 1;

  if (frame[1] == id){
    Serial.println();
    Serial.println("Receive frame");
    Serial.print("Header    : ");
    Serial.print(frame[1]);
    Serial.println(frame[2]);
    Serial.print("Frame No. : ");
    Serial.println(num);
    Serial.print("Data    : ");
    for(int i = 3; frame[i] != '\0'; i++){
      if(frame[i] == flag){ // Last one
        break;
      }else if(frame[i] == escape){ //Found escape
        data += frame[++i];
      }else{
        data += frame[i];
      }
    }
    Serial.println(data);
    Serial.print("Checking   : ");
    Serial.println("WTF");

    if(1){  // If checksum Pass...
      Serial.println("Received");
      Serial.println();
      Serial.print("Send ACK");
      Serial.println(ack);
    }else{
      
    }
    
  }else{
    Serial.println(frame);
  }
}

void flushRx(){
  while(popeyeza12.available())
    uint8_t tmp = popeyeza12.read();
}
