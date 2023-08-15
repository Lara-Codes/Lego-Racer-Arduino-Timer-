    // Serial.println(state);

      // If the timer displays 00:00 and the button is pressed, change state to 1 (running).
      if(digitalRead(sensorOnePin) == HIGH){
        state = 1; 
      }

      if(state==0){ // If the state is 0, display 00:00. 
        resetTimer();
      } else if(state==1){ // if the state is 1, run timer. 
        drawTime(); 
      }else{
        displayEndTime(); // If the state is 2, display most recent value.
      }
      
}

// Stopwatch function. If the timer is in state 0 (00:00) and the sensor is triggered, the program will trigger this function. 
void drawTime(){
  Serial.println(state); // For debugging. Can delete later. 
    for (uint16_t counter = 0; counter < 9999; counter ++) { // uint16_t is an unsigned 16 bit integer data type 
      // Storing the most current values of each digit on the LED display. These values are what the stopwatch will "freeze" to when stopped. 
      dig0=counter/1000; 
      dig1 = (counter/100) % 10; 
      dig3= (counter/10) % 10;
      dig4=counter % 10;

      // Writing the digits to the LED display 
      matrix.writeDigitNum(0, (counter / 1000), drawDots);
      matrix.writeDigitNum(1, (counter / 100) % 10, drawDots);
      matrix.drawColon(drawDots);
      matrix.writeDigitNum(3, (counter / 10) % 10, drawDots);
      matrix.writeDigitNum(4, counter % 10, drawDots);
      matrix.writeDisplay();

      delay(8); // This statement is needed or else the timer goes waaay too fast. THIS IS FOR THE FOR-LOOP NOT THE HARDWARE! 

      // If the sensor is triggered while the stopwatch is runing, increase the state to 2 to stop the numbers and display the most recent digits. Then break out of the loop. 
      if(digitalRead(sensorTwoPin) == HIGH) {
          state = 2;
          Serial.println(counter);
          break;
      }
  }
}

// When the state is equal to 2 (ie the sensor was triggered while the stopwatch was running), display the digits stored in dig0dig1:dig3dig4 (freeze the timer).
void displayEndTime(){

  // String str = dig0 + " ";
  // String str2 = str + dig1; 
  // String str3 = str2 + " ";
  // String str4 = str3 + dig3; 
  // String str5 = str4 + " ";
  // String str6 = str5 + dig4;
  // Serial.println(str6);

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
