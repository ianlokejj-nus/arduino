
#define POMODORO_DURATION 15000 // milliseconds
#define REST_DURATION 5000 // milliseconds
#define START_BUTTON 7

boolean time_to_concentrate;
boolean start;
unsigned long start_time;
unsigned long current_time;

void setup() {
 Serial.begin(9600);
 
 pinMode(START_BUTTON, INPUT_PULLUP);
 
 time_to_concentrate = true;
 start = false;
 start_time = millis();
 current_time = millis();
}

void loop() {
  int start_button_status = digitalRead(START_BUTTON);
  if (time_to_concentrate) {
    if (start_button_status == LOW) {
      // Run for 25 mins
      current_time = millis();
      while(current_time < start_time + POMODORO_DURATION) {
        current_time = millis();
        Serial.println("Concetrate!!!!");
        delay(1000);
      }
      // Completed 25 mins... time to rest
      time_to_concentrate = false;
      start = false;
    } else {
      //  Indicate ready to start
      start_time = millis();
      Serial.println("Ready to start concentrating?");
      delay(1000);
      
    }
  } else {
    if (start_button_status == LOW){
      // Rest for 5 mins
      current_time = millis();
      while(current_time < start_time + REST_DURATION) {
        current_time = millis();
        Serial.println("Resting in progress!");
        delay(1000);
      }
      time_to_concentrate = true;
      start = false;
    } else {
      // Indicate ready to start
      start_time = millis();
      Serial.println("Ready to Rest?");
      delay(1000);
    }
  }
}
