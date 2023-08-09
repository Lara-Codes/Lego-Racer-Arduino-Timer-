// Include for boolean operations 
#include <stdbool.h>

// Includes for the LED display 
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// Pins connected to each of the 2 sensors 
const int sensorOnePin = 2; 
const int sensorTwoPin = 16;

// LED display 
Adafruit_7segment matrix = Adafruit_7segment();

// Indicates if you want the dots to be displayed on the LED board (ex. 11:11)
bool drawDots; 

// Each digit on LED display 
int dig0, dig1, dig3, dig4; 

int state;

void setup() {
    // Sensor 1 initializing
    pinMode(sensorOnePin, INPUT_PULLUP); // INPUT_PULLUP -> sensor HIGH when triggered. 

    // Sensor 2 initializing
    pinMode(sensorTwoPin, INPUT_PULLUP);

    // Enable dots in time (ex: 11:11)
    drawDots = true; 

  // I2C communication port for LED display 
    matrix.begin(0x70);

    // Initializing each digit on LED display to 0 
    dig0 = 0, dig1=0, dig3=0, dig4 = 0; 

    // State of timer. Can be 0 (00:00), 1 (timer running), or 2 (Timer stopped).
    state = 0;

    // Serial monitor for debugging. Can delete later. 
    Serial.begin(9600);
}

void loop() {
    // Printing state of timer for debugging. Can delete later. 
    Serial.println(state);

    if(digitalRead(sensorOnePin) == HIGH){ // If the first sensor is triggered, 
      delay(500); // Short delay. WILL NOT WORK without delay. Can play around with this number, but it gets buggy without a short delay because it triggers the button multiple times in one click.
      if(state==2){ // If the state is 2 and the button was clicked, reset the state to 0 (to display 00:00).
        state=0;
      }else{ // If the sensor is triggered, increase the state value.
        state++;
      }
    }

      if(state==0){ // If the state is 0, display (00:00).
        resetTimer(); 
      }else if(state==1){ // If the state is 1, enter function with the for-loop for the timer. Stays here in this function until the sensor is triggered again, in which case the
      // state is increased again. 
        drawTime();
      } else{ // If the state is 2, displays the values stored in dig0dig1 : dig3dig4. These values store the most current value displayed on the timer. 
        displayEndTime();
      }
}

// Stopwatch function. If the timer is in state 0 (00:00) and the sensor is triggered, the program will trigger this function. 
void drawTime(){
  Serial.println(state); // For debugging. Can delete later. 
    for (uint16_t counter = 0; counter < 9999; counter ++) {
      // Storing the values of each digit on the LED display 
      dig0=counter/1000; 
      dig1 = counter/100; 
      dig3= counter/10;
      dig4=counter%10;

      // Writing the digits to the LED display 
      matrix.writeDigitNum(0, (counter / 1000), drawDots);
      matrix.writeDigitNum(1, (counter / 100) % 10, drawDots);
      matrix.drawColon(drawDots);
      matrix.writeDigitNum(3, (counter / 10) % 10, drawDots);
      matrix.writeDigitNum(4, counter % 10, drawDots);
      matrix.writeDisplay();
      delay(8); // This statement is needed or else the timer goes insanely fast. You can play around with this as necessary. However, I found that the 8ms delay 
      // made the loop match up closely with an actual stopwatch. 

      // If the sensor is triggered while the stopwatch is runing, increase the state to 2 to stop the numbers and display the most recent digits. Then break out of the loop. 
      if(digitalRead(sensorOnePin) == HIGH) {
          state++;
          delay(1000); // Again, this delay is necessary to avoid buginess but the exact value can probably be played around with.
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
