#include <Wire.h>
#define LED 13
#define SLAVE_ADDRESS 0x04 
#define NUMBER_OF_DATA_SEND 3
int number = 0;
int state = 0;
int data[3] ={1,2,3}; // array for received data
bool dataSendReadyFlag = false;
//int counter = 0;

void setup() {
pinMode(LED, OUTPUT);
Serial.begin(9600); // start serial for output
// initialize i2c as slave
Wire.begin(SLAVE_ADDRESS);

// define callbacks for i2c communication
Wire.onReceive(receiveData);
Wire.onRequest(sendData);

Serial.println("I2C communication is Ready!");
}

void loop() {
delay(1000);
}

// callback for received data
void receiveData(int byteCount){
static int howManyDataArrived =0;

while(Wire.available()) {
  howManyDataArrived++;
  number = Wire.read();
  // check number of data 
  if (howManyDataArrived == NUMBER_OF_DATA_SEND)
    dataSendReadyFlag = true;
  //data[howManyDataArrived-1] = number;
  Serial.print("data received: ");
  Serial.println(number);

/*if (number == 1){

if (state == 0){
digitalWrite(LED, HIGH); // set the LED on
state = 1;
}
else{
digitalWrite(LED, LOW); // set the LED off
state = 0;
}
}*/
}
}

// callback for sending data
void sendData(){
  static int sendDataNum = 0;
  
  if (dataSendReadyFlag == true)
  {
    Wire.write(data[sendDataNum]);
    Serial.println("sending data:");Serial.print(sendDataNum);
    sendDataNum = sendDataNum + 1;
    if (sendDataNum > 2)
    {
      dataSendReadyFlag = false;
      sendDataNum = 0;
    }
    
  }
  else
   Serial.println("i'm being called but NOT sending data");
 
}
