/* LED Matrix Code
 *  By Archie Roques, July 2017
 *  Released under CC-BY-SA-NC Creative Commons 3.0 License
 */

//define the pins for one shift register, the active high one
int latchPin = 14;
int clockPin = 12;
int dataPin = 16;
//define pins for the other register, the active low one
//(the clock pin is shared so only defined once)
int neglatchPin = 5;
int negdataPin = 4;
//define a pin for the onboard LED used for indication
int ledPin = 2;

//define a string to store the data from serial
String serialData = "";
//define a boolean variable to track if the data is totally recieved
boolean alldataRecieved = false;
//define a counter variable for when data is recieved
int counter = 0;
//define a rownumber variable, starts at 0 by default
int rowNumber = 0;

int i;

//set up eight empty bytes to keep the data in, one for each row 
byte row0 = B11111111;
byte row1 = B00000000;
byte row2 = B00000000;
byte row3 = B00000000;
byte row4 = B00000000;
byte row5 = B00000000;
byte row6 = B00000000;
byte row7 = B00000000;



//set up an array of those arrays, so we can track which one we put the data into later


void setup() {
  //set all the pin modes as output (there are no input pins for the matrix)
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(neglatchPin, OUTPUT);
  pinMode(negdataPin, OUTPUT);

  //begin a serial port at a baudrate of 115200b/s
  Serial.begin(9600);
  //Serial.println("Booted!");

  //reserve 250 bytes for the data:
  //serialData.reserve(250);

  //turn the LED pin to high, to turn it off (the ESP module uses an active-LOW configuration)
  digitalWrite(ledPin, HIGH);

  

}

void loop() {

  serialEvent();

  delay(100);
  //this will basically just continually update the registers in turn with the byte data
//  Serial.println("Row:");
  Serial.print(row3, BIN);

  for (int col = 0; col < 8; col++) {
    
    byte colshifter = B11111111;
    byte rowshifter = B00000000;
    bitClear(colshifter, col);
  if ((bitRead(row0, col)) == 1) {
    bitSet(rowshifter, 0);
  }
  if ((bitRead(row1, col)) == 1) {
    bitSet(rowshifter, 1);
  }
  if ((bitRead(row2, col)) == 1) {
    bitSet(rowshifter, 2);
  }
  if ((bitRead(row3, col)) == 1) {
    bitSet(rowshifter, 3);
  }
  if ((bitRead(row4, col)) == 1) {
    bitSet(rowshifter, 4);
  }
  if ((bitRead(row5, col)) == 1) {
    bitSet(rowshifter, 5);
  }
  if ((bitRead(row6, col)) == 1) {
    bitSet(rowshifter, 6);
  }
  if ((bitRead(row7, col)) == 1) {
    bitSet(rowshifter, 7);
  }

   updateShifts(rowshifter, colshifter);
  //updatenegativeShifts(colshifter);
  }
  

}



/*
 *     
    byte colshifter = B11111111;
    byte rowshifter = B00000000;
    //(colshifter >> col) = 0x00;
    bitClear(colshifter, col)
  if (bitRead(row0, col)) == 1 {
    //(rowshifter >> 0) = 0x01;
    bitSet(rowshifter, 0);
  }
  
  }

 */

//create a function to update the register with data as a parameter
void updateShifts(byte data, byte negdata) {
  //turn outputs off
  digitalWrite(latchPin, LOW);
  digitalWrite(neglatchPin, LOW);
  //shift the data out
  shiftOut(dataPin, clockPin, LSBFIRST, data);
  shiftOut(negdataPin, clockPin, LSBFIRST, negdata);
  //turn outputs back on
  digitalWrite(latchPin, HIGH);
  digitalWrite(neglatchPin, HIGH);
}


//create a function to update the other register with data as a parameter
void updatenegativeShifts(byte data) {
  //turn outputs off
  digitalWrite(neglatchPin, LOW);
  //shift the data out
  shiftOut(negdataPin, clockPin, LSBFIRST, data);
  //turn outputs back on
  digitalWrite(neglatchPin, HIGH);
}


//create a special function, which will run every time new data is recieved automatically
void serialEvent() {
  digitalWrite(ledPin, LOW);
  //delay(1000);
  digitalWrite(ledPin, HIGH);
  //while there is a serial port open
  while (Serial.available()) {
    byte inByte = Serial.read();
    Serial.print(inByte, BIN);
    switch(counter) {
      case 0:
        row0 = inByte;
        break;
      case 1:
        row1 = inByte;
        break;
      case 2:
        row2 = inByte;
        break;
      case 3:
        row3 = inByte;
        break;
      case 4:
        row4 = inByte;
        break;
      case 5:
        row5 = inByte;
        break;
      case 6:
        row6 = inByte;
        break;
      case 7:
        row7 = inByte;
        break;
      default:
        Serial.print("This should never happen..");
        break;
    }
    
    counter++;
    if(counter == 8){
      counter = 0;
  }
}
}
