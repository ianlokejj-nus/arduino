#include <Adafruit_NeoPixel.h>

#define START_BUTTON 7
#define START 0
#define CONCENTRATE 1
#define CONCENTRATE_OVER 2
#define REST 3
#define CONCENTRATION_DURATION 15000 // milliseconds
#define REST_DURATION 5000 // milliseconds
#define PIXELS 16
#define PIN 6
#define BUTTON_DELAY 2000

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t state;
uint16_t start_time;
uint16_t current_time;
boolean paused;
uint16_t cycle_time;
uint8_t current_pixel = 0;
uint8_t colour_counter = 0;
uint16_t pixel_freq_concentration = CONCENTRATION_DURATION / PIXELS;

void setup(){
  Serial.begin(9600);
  pinMode(START_BUTTON, INPUT_PULLUP);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(5);  
  
  state = START;
  start_time = millis();
  paused = false;
  Serial.println("In Start State");
}

void loop(){
  
  if (state==START){
   if (digitalRead(START_BUTTON)==LOW){
     state=CONCENTRATE;
     start_time = current_time = millis();
     cycle_time = 0;
     colorWipe(strip.Color(255, 0, 0), 50);
     Serial.println("Proceeding to Concentrate");
     Serial.println("Start time:");
     Serial.println(start_time);
     Serial.println("End time:");
     Serial.println(start_time + CONCENTRATION_DURATION + BUTTON_DELAY);
     delay(BUTTON_DELAY);
   }
  } else if (state==CONCENTRATE){
    if (digitalRead(START_BUTTON)==LOW){
      Serial.println("Pause Button Pressed");
      paused = !paused;
      delay(BUTTON_DELAY); 
    }
    if (!paused){
      if(current_time < start_time + CONCENTRATION_DURATION + BUTTON_DELAY + 1000){ // The additional 1 sec is added to allow the last pixel to light up 
        current_time = millis();
        Serial.println("current_time");
        Serial.println(current_time);
        
        //Update pixel status
       if (current_time - cycle_time > pixel_freq_concentration){
          //Serial.println(cycle_time);
          cycle_time = current_time;
          change_pixel_colour();
          Serial.println("Lighting up LED");      
        }
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

void change_pixel_colour() {
    uint8_t pixel_colour;
    
    
    Serial.println(current_pixel);
    //colour_counter  = colour_counter + 1;
    //pixel_colour = Wheel((colour_counter) & 255);
    
    //for(int i=0; i<current_pixel; i++){
      strip.setPixelColor(current_pixel, strip.Color(0, 255, 0));
    //}
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
