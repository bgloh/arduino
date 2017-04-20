// I2C basic callback
// Receiving end returns what it received to sending end.


#include <Wire.h>
#define LED 13
#define SLAVE_ADDRESS 0x04 
#define NUMBER_OF_DATA_SEND 3

// global variables
int receivedData = 0;
int data[NUMBER_OF_DATA_SEND];  // array for received data
bool dataSendReadyFlag = false;


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
  howManyDataArrived++; // increase data index
  receivedData = Wire.read(); // read data from i2c
  data[howManyDataArrived-1] = receivedData; // save data
  
  Serial.print("data received: ");
  Serial.println(*(data + howManyDataArrived -1));

  // check number of data and do something 
  if (howManyDataArrived == NUMBER_OF_DATA_SEND)
  {
    dataSendReadyFlag = true;
    howManyDataArrived = 0; // reset
    }
  

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
    Serial.print("sending data:");Serial.println(sendDataNum + 1);
    sendDataNum = sendDataNum + 1;
    if (sendDataNum == NUMBER_OF_DATA_SEND)
    {
      dataSendReadyFlag = false;
      sendDataNum = 0;
    }
    
  }
  else
  {
    Serial.println("i'm being called but NOT ready to send data");
     Wire.write(0xFF);
  }
 
}
