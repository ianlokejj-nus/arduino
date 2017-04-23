#include <Event.h>
#include <Timer.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define PIXELS 16
#define POMODORO_DURATION 150000 // milliseconds
#define REST_DURATION 300000 // milliseconds TODO:: Implement Rest logic

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);
long pixel_pomodoro_interval = POMODORO_DURATION / PIXELS;
uint8_t current_pixel = 0;
uint8_t rainbow_type;
uint8_t colour_counter = 0;
Timer t;

void setup() {
  Serial.begin(9600);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(5);

  t.every(pixel_pomodoro_interval, light_up_pixel, PIXELS); // Run the pomodoro cycle
  t.every(200, change_pixel_colour); // Change the colour of the pixel
}

void loop() {
  t.update();
}

void change_pixel_colour() {
    uint32_t pixel_colour;
    colour_counter  = colour_counter + 1;
    pixel_colour = Wheel((colour_counter) & 255);
    for(int i=0; i<current_pixel; i++){
      strip.setPixelColor(i, pixel_colour);
    }
    strip.show();
}

void light_up_pixel() {
  current_pixel++;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

  WheelPos = 255 - WheelPos;
  Serial.println(WheelPos);
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

