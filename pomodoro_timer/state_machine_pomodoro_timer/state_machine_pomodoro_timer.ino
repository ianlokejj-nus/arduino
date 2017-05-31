#include <Adafruit_NeoPixel.h>

#define START_BUTTON 7
#define START 0
#define CONCENTRATE 1
#define CONCENTRATE_OVER 2
#define REST 3
#define POMODORO_DURATION 5000 // milliseconds
#define REST_DURATION 5000 // milliseconds

uint8_t state;
uint16_t start_time;
uint16_t current_time;
boolean paused;

void setup(){
  Serial.begin(9600);
  pinMode(START_BUTTON, INPUT_PULLUP);
  
  state = START;
  start_time = millis();
  paused = false;
  Serial.println("In Start State");
}

void loop(){
  
  if (state==START){
   if (digitalRead(START_BUTTON)==LOW){
     state=1;
     start_time = millis();
     Serial.println("Proceeding to Concentrate");
     delay(2000);
   }
  } else if (state==CONCENTRATE){
    if (digitalRead(START_BUTTON)==LOW){
      Serial.println("Pause Button Pressed");
      paused = !paused;
      delay(2000); 
    }
    if (!paused){
      if(current_time < start_time + POMODORO_DURATION){
        current_time = millis();
      } else {
         state=CONCENTRATE_OVER;
         current_time = millis();
         start_time = millis();
        Serial.println("Time's up, Time to Rest"); 
      }
    }
  } else if (state==CONCENTRATE_OVER){
      if(current_time < start_time + REST_DURATION){
        current_time = millis();
      } else {
         state=START;
        Serial.println("Time's up, Back to Concentrating"); 
      }
  } else {
    Serial.println("Not suppose to be here");
  }
}


