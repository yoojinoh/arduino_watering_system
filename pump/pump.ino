#include <IRremote.h> //works with v2.6.0, latest doesn't work
#include <ir_Lego_PF_BitStreamEncoder.h>


#include <Adafruit_Sensor.h>
#include <dht.h>
#include <virtuabotixRTC.h> //Library used
#include <IRremote.h>  //including infrared remote header file     

// constants won't change
const int RELAY_PIN = A0;  // the Arduino pin, which connects to the IN pin of relay
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
int RECV_PIN = 12; // the pin where you connect the output pin of IR sensor     

virtuabotixRTC myRTC(6, 7, 8); //If you change the wiring change the pins here also

// DHT11 sensor
dht DHT;
#define DHT11_PIN 4

//IRremote
IRrecv irrecv(RECV_PIN);     
decode_results results;  

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int printedTemp = 0;
int flagIR = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();     
  // initialize digital pin A5 as an output.
  pinMode(RELAY_PIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  
 // Set the current date, and time in the following format:
 // seconds, minutes, hours, day of the week, day of the month, month, year
 //myRTC.setDS1302Time(15, 13, 18, 1, 2, 5, 2022); //Here you write your actual time/date as shown above 
 //but remember to "comment/remove" this function once you're done
 //The setup is done only one time and the module will continue counting it automatically

}

// the loop function runs over and over again forever
void loop() {
  // update time
  myRTC.updateTime();

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    digitalWrite(RELAY_PIN, HIGH); // turn on pump 5 seconds
  }
  else {
  digitalWrite(ledPin, HIGH);
  digitalWrite(RELAY_PIN, LOW);  // turn off pump 5 seconds
  }
  if (myRTC.seconds==30 && printedTemp==0) {
    Serial.print("Current Date / Time: ");
    Serial.print(myRTC.dayofmonth); //You can switch between day and month if you're using American system
    Serial.print("/");
    Serial.print(myRTC.month);
    Serial.print("/");
    Serial.print(myRTC.year);
    Serial.print(" ");
    Serial.print(myRTC.hours);
    Serial.print(":");
    Serial.print(myRTC.minutes);
    Serial.print(":");
    Serial.println(myRTC.seconds);
    
    int chk = DHT.read11(DHT11_PIN);
    Serial.print("Temperature = ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    printedTemp=1;
  }
  else if (myRTC.seconds!=30){
  printedTemp=0;
}


if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.     
{     
 int readResults = results.value;// Results of decoding are stored in result.value  
 if (readResults==19899){ // play button
     digitalWrite(RELAY_PIN, HIGH); // turn on pump 1 seconds
    delay(5000);
 }

 irrecv.resume(); // Restart the ISR state machine and Receive the next value     
} 

}





