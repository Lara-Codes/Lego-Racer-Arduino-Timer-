// Include for boolean operations 
#include <stdbool.h>

// Includes for the LED display 
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// Pins connected to each of the 2 sensors 
const int sensorOnePin = 9; 
const int sensorTwoPin = 10; 

// LED display initialization
Adafruit_7segment matrix = Adafruit_7segment();

// Indicates if you want the dots to be displayed on the LED board (ex. 11:11). Initialized to true in setup().
bool drawDots; 

// Each digit on LED display. Skips 2 because digit2 represents the colon in the time. 
int dig0, dig1, dig3, dig4; 


// State of timer. Can be 1 (timer running), or 2 (Timer stopped). State determined by sensor/button input in loop(). Ommitted state 0 
// after it turns on initially (display 00:00) but can be re-included with a reset button as necessary. 
int state;

void setup() {
    // Sensor 1 initializing
    pinMode(sensorOnePin, INPUT); // INPUT_PULLUP -> sensor HIGH when triggered. 

    // Sensor 2 initializing
    pinMode(sensorTwoPin, INPUT);

    // Enable dots in time (ex: 11:11)
    drawDots = true; 

  // I2C communication port for LED display 
    matrix.begin(0x70);

    // Initializing each digit on LED display to 0. Skips 2 because digit2 represents the colon in the time. 
    dig0 = 0, dig1=0, dig3=0, dig4 = 0; 

    // Initialize state of timer. Can be 0 (00:00), 1 (timer running), or 2 (Timer stopped).
    state = 0;

    // Serial monitor for debugging. Can delete later. 
    Serial.begin(9600);
}

void loop() {
    // Printing state of timer for debugging. Can delete later. 
    // Serial.println(state);

      // If the timer displays 00:00 and the button is pressed, change state to 1 (running).
      if(digitalRead(sensorOnePin) == HIGH){
        state = 1; 
      }

      if(state==0){ // If the state is 0, display 00:00. Again, can connect a reset button to enter this state. As of now, 
          // it only displays 00:00 when the Arduino intitially turns on. 
        resetTimer();
      } else if(state==1){ // if the state is 1, run timer. 
        drawTime(); // The state may become 2 and display end time only in this function.
      }else{
        displayEndTime(); // If the state is 2, display most recent value stored in dig0, dig1, dig3, dig4. Digits set in drawTime() function.
      }
      
}

// Stopwatch function. If the first sensor is triggered, the program will enter this function. 
void drawTime(){
    for (uint16_t counter = 0; counter < 9999; counter ++) { // uint16_t is an unsigned 16 bit integer data type 
      // Storing the most current values of each digit on the LED display. These values are what the stopwatch will "freeze" to when stopped. 
      // The counter represents the time displayed on the stop watch. The code below extracts each digit from the overal number. For 
        // example, if the counter is 1456, dig0 will hold 1, dig1 will hold 4, dig3 holds 5, and dig4 holds 6. 
        dig0=counter/1000; 
      dig1 = (counter/100) % 10; 
      dig3= (counter/10) % 10;
      dig4=counter % 10;

      // Writing each digits to the LED display. 
      matrix.writeDigitNum(0, (counter / 1000), drawDots);
      matrix.writeDigitNum(1, (counter / 100) % 10, drawDots);
      matrix.drawColon(drawDots);
      matrix.writeDigitNum(3, (counter / 10) % 10, drawDots);
      matrix.writeDigitNum(4, counter % 10, drawDots);
      matrix.writeDisplay();

      delay(8); // This statement is needed or else the timer goes waaay too fast. THIS IS FOR THE FOR-LOOP NOT THE HARDWARE! 
        // I compared the speed of the program to a real stopwatch, and a delay of 8ms makes the loop closest to 1 loop = 1s. 
        
      // If the second sensor is triggered while the stopwatch is runing, increase the state to 2 and break out of the loop. 
        // When the state is 2, it displays the digits stored in dig0 - dig4 (Freezes the stopwatch). Calls function displayEndTime() 
        // in the main loop when state is 2. 
      if(digitalRead(sensorTwoPin) == HIGH) {
          state = 2;
          break;
      }
  }
}

// When the state is equal to 2 (ie the sensor was triggered while the stopwatch was running), display the digits stored in dig0dig1:dig3dig4 (freeze the timer).
void displayEndTime(){
    matrix.writeDigitNum(0, dig0, drawDots);
    matrix.writeDigitNum(1, dig1, drawDots);
    matrix.drawColon(drawDots);
    matrix.writeDigitNum(3, dig3, drawDots);
    matrix.writeDigitNum(4, dig4, drawDots);
    matrix.writeDisplay();
}

// When the state is equal to 0, display 00:00. This is the default state and will also display 00:00 after the reset button is triggered, 
void resetTimer(){
  matrix.writeDigitNum(0, 0, drawDots);
  matrix.writeDigitNum(1, 0, drawDots);
  matrix.writeDigitNum(3, 0, drawDots);
  matrix.writeDigitNum(4, 0, drawDots);
  matrix.writeDisplay();
}
