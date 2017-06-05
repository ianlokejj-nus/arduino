#include <Adafruit_NeoPixel.h>

#define START_BUTTON 7
#define START 0
#define CONCENTRATE 1
#define CONCENTRATE_OVER 2
#define REST 3
#define CONCENTRATION_DURATION 1500000 // milliseconds
#define REST_DURATION 300000 // milliseconds
#define PIXELS 16
#define PIN 6
#define BUTTON_DELAY 2000

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t state;
unsigned long start_time;
unsigned long current_time;
boolean paused;
unsigned long cycle_time;
uint8_t current_pixel = 0;
uint8_t colour_counter = 0;
uint32_t pixel_freq_concentration = CONCENTRATION_DURATION / PIXELS;
uint32_t pixel_freq_rest = REST_DURATION / PIXELS;
unsigned long pause_duration = 0;
unsigned long start_pause_time = 0;
unsigned long end_pause_time = 0;

void setup(){
  Serial.begin(9600);
  pinMode(START_BUTTON, INPUT_PULLUP);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(1);  
  
  state = START;
  start_time = millis();
  paused = false;
  colorWipe(strip.Color(255, 0, 0), 50);
  
  Serial.println("In Start State");
}

void loop(){
  
  if (state==START){
   if (digitalRead(START_BUTTON)==LOW){
     colorWipe(strip.Color(255, 255, 0), 50);
     state=CONCENTRATE;
     start_time = current_time = millis();
     cycle_time = 0;
     pause_duration = 0;
     current_pixel = 0;
     Serial.println("Proceeding to Concentrate");
     Serial.println("Start time:");
     Serial.println(start_time);
     Serial.println("End time:");
     Serial.println(start_time + CONCENTRATION_DURATION + BUTTON_DELAY);
     Serial.println("pixel_freq_concentration");
     Serial.println(pixel_freq_concentration);
     delay(BUTTON_DELAY);
   }
  } else if (state==CONCENTRATE){
    if (digitalRead(START_BUTTON)==LOW){
      Serial.println("Pause Button Pressed");
      paused = !paused;
      
      
      if(paused){
        // get the start of the pause time
         start_pause_time = millis();
      } else {
        // Unpausing
        end_pause_time = millis();
        pause_duration = end_pause_time - start_pause_time;
      }
      delay(BUTTON_DELAY);
    }
    if (!paused){
      if(current_time < start_time + CONCENTRATION_DURATION + BUTTON_DELAY + pause_duration + 700){ // The additional 1 sec is added to allow the last pixel to light up 
        current_time = millis();
        //Serial.println("current_time");
        //Serial.println(current_time);
        
        //Update pixel status
       if (current_time - cycle_time > pixel_freq_concentration){
          //Serial.println(cycle_time);
          cycle_time = current_time;
          change_pixel_colour(strip.Color(0, 255, 0));
          Serial.println("Lighting up LED");      
        }
      } else {
        Serial.println("Time's up, Time to Rest");
          state=CONCENTRATE_OVER;
          // Play some nice visuals... This is where I want to wind down my work
          colorWipe(strip.Color(255, 255, 255), 50);
          delay(BUTTON_DELAY);
      }
    }
  } else if (state==CONCENTRATE_OVER){
    if(digitalRead(START_BUTTON)==LOW){
      colorWipe(strip.Color(0, 0, 255), 50);
       Serial.println("Proceeding to Rest");
       delay(BUTTON_DELAY);
       state=REST;
       current_time = millis();
       start_time = millis();
       current_pixel = 0;
    }
  }
  
    else if (state==REST){
      // Have a start button to start resting
      if(current_time < start_time + REST_DURATION){
        current_time = millis();
        // follow the same concept as concentration except no pausing
        //Update pixel status
        if (current_time - cycle_time > pixel_freq_rest){
          //Serial.println(cycle_time);
          cycle_time = current_time;
          change_pixel_colour(strip.Color(0, 128, 255));
          Serial.println("Lighting up LED");      
         }        
      } else {
         state=START;
        Serial.println("Time's up, Back to Concentrating");
        colorWipe(strip.Color(255, 255, 255), 50);
      }
  } else {
    Serial.println("Not suppose to be here");
  }
}

void change_pixel_colour(uint32_t c) {
    uint8_t pixel_colour;
    Serial.println(current_pixel);
    strip.setPixelColor(current_pixel, c);
    strip.show();
    current_pixel++;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
