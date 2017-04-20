// I2C communication protocol
// data[0] : Service
// data[1] : characteristic
// data[2] : payload MSB
// data[3] : payload LSB


#include <Wire.h>
#define LED 13
#define SLAVE_ADDRESS 0x04 
#define NUMBER_OF_DATA_SEND 4
#define NUM_SERVICE 2 // number of service
#define NUM_CHAR 3   // number of characteristics

// global variables
int receivedData = 0;
int data[NUMBER_OF_DATA_SEND];  // array for received data
bool dataReadyToReadFlag = false;
bool dataReadyToSendFlag = false;
bool excutionDoneFlag = false;
int serviceList[NUM_SERVICE] ={ 0x0A,0x0B };
int charListOfservice1[NUM_CHAR] = {1,2,9}; // characteristics list of service 1
int charListOfservice2[NUM_CHAR] = {3,5,7}; // characteristics list  of service 2
int* ptr_charList[NUM_SERVICE] = {charListOfservice1,charListOfservice2};



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
  if (dataReadyToReadFlag == true)
    { 
       
       I2CmessageEncoding(data);

    }
  
delay(100);
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
    dataReadyToReadFlag = true;
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
  
  if (dataReadyToSendFlag == true)
  {
    Wire.write(data[sendDataNum]);
    Serial.print("sending data:");Serial.println(sendDataNum + 1);
    sendDataNum = sendDataNum + 1;
    if (sendDataNum == NUMBER_OF_DATA_SEND)
    {
      dataReadyToSendFlag = false;
      sendDataNum = 0;
    }
    
  }
  else
  {
    Serial.println("i'm being called but NOT ready to send data");
     Wire.write(0xFF);
  }
 }

 // message encoding
 void I2CmessageEncoding(int * message) 
 {  
    
    int* ptr_characterisitcListFound;
    int serviceFound = 0;
    int characteristicsFound = 0;
    Serial.println("ha ha ha");
    Serial.println(*message);
    Serial.println(*(message+1));

    // find service and relevant characteristic list
    for (int i=0; i < NUM_SERVICE; i++)
    {
      if ( *message == *(serviceList+i))
      {
         serviceFound =  *(serviceList+i);
         ptr_characterisitcListFound = *(ptr_charList+i);
         Serial.print(serviceFound );
         Serial.println("  <=== service match found");
          Serial.print(*ptr_characterisitcListFound );
         Serial.println("  <=== char list match found");
         
      }
    }

    for (int j=0;j<NUM_CHAR;j++)
    {
      if ( *(ptr_characterisitcListFound+j) == *(message+1))
            {
               characteristicsFound =*(ptr_characterisitcListFound+j);
               Serial.println("found characterisitcs == > ");
               Serial.println(characteristicsFound);
             }
    }
    
 
       
  /*  switch(*message)
    {
    case 0:
      digitalWrite(LED,HIGH);break;
    case 1:
      digitalWrite(LED,LOW);break;
    } // switch-end */
  dataReadyToReadFlag = false;
 }

