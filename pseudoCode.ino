#include <stdbool.h>
#include <LiquidCrystal.h>

// Pins connected to each of the 2 sensors 
const int sensorOnePin = 1; 
const int sensorTwoPin = 2; 
const int buttonPin = 3; // connected to "reset timer" button

// Storing times each sensor has been triggered 
unsigned long startTime = 0; 
unsigned long endTime = 0; 
unsigned long elapsedTime = 0; 

// Check if the first sensor has been triggered  
bool firstSensorTriggered = false; 

// Is the timer running or stopped? 
bool timerRunning = false; 

// Connect digital timing clock 
LiquidCrystal lcd(4, 5, 6, 7, 8); // numbers of interface pins of digital display 

void setup(){
    // Sensor 1 
    pinMode(1, INPUT_PULLUP); 

    // Sensor 2 
    pinMode(2, INPUT_PULLUP); //INPUT_PULLUP internally connects the pin to a HIGH logic level. 
    
    // Button 
    pinMode(3, INPUT_PULLUP); 

    // Initialize communication protocol between external digital timer here ()
    lcd.begin(16, 2); // LCD rows/columns 
    lcd.clear(); 
    lcd.print("On your mark, get set..."); //Initial message 
    lcd.display(): 

    Serial.begin(9600); //start serial connection
}

void loop(){

    lcd.setCursor(0, 1); // Set cursor to column 0, row 1 

    if(digitalRead(sensorOnePin) == LOW && !firstSensorTriggered){ // If the first sensor is triggered for the first time, 
        firstSensorTriggered = true; // mark as triggered
        timerRunning = true; // Set timer running equal to true 
        startTime = millis(); // Start timer 
        delay(400);
        
        // Send signal to external timer here using timer modules 
    }

    if(timerRunning == true){
        lcd.setCursor(0, 1);
        lcd.print((millis()- startTime) / 1000.0); // Outputting # of seconds elapsed since beginning 
    }

    // If second sensor triggered and the first one was triggered, calculate elapsed time. 
    if(digitalRead(sensorTwoPin) == LOW && firstSensorTriggered) {
        endTime = millis(); 
        timerRunning = false; // Set timer running to false 
        elapsedTime = endTime - startTime; 
        delay(100);
        // Stop counting & display elapsed time on timer here 
    }

    // If the button is pressed, reset start/end time and firstSensorTriggered. 
    if(digitalRead(buttonPin) == LOW){
        startTime = 0; 
        endTime = 0; 
        firstSensorTriggered = false; 
        timerRunning = false;
        delay(100); // make sure to not trigger it twice. Small delay. 
        
        // Set timer equal to 0 
    }

    // May need to format time 

}