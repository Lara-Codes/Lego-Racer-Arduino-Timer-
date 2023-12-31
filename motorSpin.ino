#include <stdbool.h> // Arduino nano 

unsigned long timeDelay = 20000; // Time delay by 20s 
unsigned long timeSpinning = 60000; // Spin for 60s 

bool motorRunning; // Is motor running? 
bool onHold; // Is the time delay in effect? 

const int sensorPin = 4; // pin connected to sensor
const int relayPin = 13; // pin connected to relay 

void setup(){
    motorRunning = false; // Initialize motorRunning 
    onHold = false; // Intialize onHold

    // Initialize pin connected to sensor as an input. LOW if its triggered.
    pinMode(sensorPin, INPUT_PULLUP);

    // Initialize pin connected to relay as an output.
    pinMode(relayPin, OUTPUT); 
    // Serial.begin(9600);
}

void loop(){

    // If sensor detected and its not moving or on the delay, turn on relay/motor.
    if(digitalRead(sensorPin) ==  HIGH && motorRunning==false && onHold==false){ 
        
        motorRunning = true; 
        // turn on relay pin 
        digitalWrite(relayPin, HIGH);

        // keep spinning for 60 seconds
        delay(timeSpinning); 

        // Turn off relay/motor 
        digitalWrite(relayPin, LOW); 

        // Indicate motor no longer running 
        motorRunning = false; 

        // Indicate delay begins 
        onHold = true; 
        delay(timeDelay); 

        // After delay, can be triggered again 
        motorRunning = false; 
        onHold = false; 
    }
}
