#include "regtable.h"
#include "swap.h"
#include "HardwareSerial.h"
#include "Arduino.h"

// define LED pin
#define LEDPIN               4

// Binary states
byte i=0;
/*
* stateLowByte is the value that will go inside the register.
* look at regtable.ino
*/
byte stateLowByte,laststate = 0 ;
/*
* declare All the coordinates setted for this mote
* they are in Byte and defined in the Setup() function
*/
byte coordinates0,coordinates1,coordinates2,coordinates3,coordinates4,coordinates5,coordinates6,coordinates7;
//Arduino Digital Output pins, declared but not used
uint8_t binaryPin[] = {8, 14, 15, 16, 17, 18, 19, 7};
//PWN output pin 
const int pwmPin=9; //arduino Digital 9 pin is connected to MAXSensor
//some variables
long value=0;
int cm=0;
int state=0;
//int inches=0;

void setup() {
  // put your setup code here, to run once:
  int i;
  //setup coordinates
  coordinates0=0x02;
  coordinates1=0x77;
  coordinates2=0x6C;
  coordinates3=0x8F;
  coordinates4=0x00;
  coordinates5=0x21;
  coordinates6=0x77;
  coordinates7=0xDB;
  
  //configure LEDPIN as output
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  
  // Configure output pins
  for(i=0 ; i<sizeof(binaryPin) ; i++)
    pinMode(binaryPin[i], OUTPUT);
  //Configure Inpute pin
  //for(i=0 ; i<sizeof(pwmPin) ; i++)
    pinMode(pwmPin, INPUT); //MAXSonar - input - digital 9
  
  // Init SWAP stack
  swap.init();
  
 
  // Transmit product code
  swap.getRegister(REGI_PRODUCTCODE)->getData();
  // Enter SYNC state
  swap.enterSystemState(SYSTATE_SYNC);
  
  // During 3 seconds, listen the network for possible commands whilst the LED blinks
  for(i=0 ; i<6 ; i++)
  {
    digitalWrite(LEDPIN, HIGH);
    delay(100);
    digitalWrite(LEDPIN, LOW);
    delay(400);
  }
  
  //Transmit Coordinates to Swap Server
  swap.getRegister(REGI_COORDINATES)->getData();
  // Transmit initial binary input states
  swap.getRegister(REGI_BININPUTS)->getData();
  // Transmit initial binary output states
  swap.getRegister(REGI_BINOUTPUTS)->getData();
  /* Switch to Rx OFF state to save battery
  * No command are received in OFF state, "LED ON" command.neither
  * Change it to RXON if you want to use swmdt or lagarto-swap in oreder 
  * to change mote carachteristics like address.
  */
  swap.enterSystemState(SYSTATE_RXOFF);

  Serial.begin(9600); //open a serial channel for MAXsonar sensor
  //Serial.println("Start");
}

void loop() {
//  Serial.println("loop");
  value = pulseIn(pwmPin,HIGH); //get MAXSonar value
  //inches = value/147; //convert the value in inches
  //cm = inches * 2.54;  //convert the inches to cm
  cm = value/58;  //convert the value to cm
  //Serial.println(cm);
  //if cm is less than 40 the value will be 1=On=HIGH
  if (cm < 40)
  {
    state= 1;
    stateLowByte = state;
    //turn on LED in Order to show the change on the Board
    digitalWrite(LEDPIN, HIGH);// LED ON
  }else
  {
    state= 0;
    stateLowByte = state;// if the cm is more than 41 the value will be 0=OFF
    digitalWrite(LEDPIN, LOW);// LED OFF
  }
  //Check if stateLowByte is changed
  if(stateLowByte!=laststate){
    /*Transmit REGI_BININPUTS data to server
    * REGI_BININPUTS is the register declared in regtable.h
    * and defined in regtable.ino*/
   swap.getRegister(REGI_BININPUTS)->getData();
   //save the last state in order to transmitt only when it change
   laststate= stateLowByte;
  }
  /*let run the loop each 2 sec to save Energy*/
  delay(2000);
}
