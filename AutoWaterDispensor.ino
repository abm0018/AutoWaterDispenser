#include <Servo.h>
#include <inttypes.h>

#define UP_BTN 2
#define DOWN_BTN 3
#define START_BTN 4
#define SERVO 5

// DELAYMS is time between reading user input and doing stuff
#define DELAYMS 200
#define STEP 50
#define MSECS_PER_OZ 1000
#define IDLE_TIMEOUT_MS 5000

#define SERVO_ON 2000
#define SERVO_OFF 1000

#define SERVO_MAX 2000
#define SERVO_MIN 1000

uint8_t ones_pins[] = {13, 14, 15, 16, 17, 18, 19}; //a,b,c,d,e,f,g
uint8_t tens_pins[] =  {12, 11, 10, 9, 8, 7, 6}; //a,b,c,d,e,f,g

Servo servo;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Auto Water Dispensor");

  pinMode(UP_BTN, INPUT_PULLUP);
  pinMode(DOWN_BTN, INPUT_PULLUP);
  pinMode(SERVO, OUTPUT);
  pinMode(START_BTN, INPUT_PULLUP);

  for (uint8_t pin=0; pin<7; pin++){
    pinMode(ones_pins[pin], OUTPUT);
    pinMode(tens_pins[pin], OUTPUT);
  }

  servo.attach(SERVO);
  servo.write(SERVO_OFF); // On startup / reset, go to OFF position, then set idle
  delay(300);
  servo.detach();
}

// If I designed a circuit board, I would use a BCD->7-segment driver chip to reduce I/O reqs.
void setDisplay(uint8_t num, uint8_t *tens_pins, uint8_t *ones_pins){ // TODO - refactor this, use a fixed array of arrays, index into array w/digit
  uint8_t active_pins[7] = {0};
  uint8_t ones = (num % 10);
  uint8_t tens = (num / 10);
  uint8_t *cur_seg_pins;
  uint8_t nums[] = {ones, tens};

  for (uint8_t i=0; i<2; i++){
    if (i==0) cur_seg_pins = ones_pins;
    else cur_seg_pins = tens_pins;

    switch (nums[i]) { 
      case 0:
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 0; // G
        break;

      case 1:
        active_pins[0] = 0; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 0; // D
        active_pins[4] = 0; // E
        active_pins[5] = 0; // F
        active_pins[6] = 0; // G
        break;

      case 2:
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 0; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 0; // F
        active_pins[6] = 1; // G
        break;

      case 3:
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 0; // E
        active_pins[5] = 0; // F
        active_pins[6] = 1; // G
        break;

      case 4:
        active_pins[0] = 0; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 0; // D
        active_pins[4] = 0; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 5:
        active_pins[0] = 1; // A
        active_pins[1] = 0; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 0; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 6:
        active_pins[0] = 1; // A
        active_pins[1] = 0; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 7:
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 0; // D
        active_pins[4] = 0; // E
        active_pins[5] = 0; // F
        active_pins[6] = 0; // G
        break;

      case 8:
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 9:
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 0; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 10: // 0xA
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 0; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 11: // 0xB
        active_pins[0] = 0; // A
        active_pins[1] = 0; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 12: // 0xC
        active_pins[0] = 1; // A
        active_pins[1] = 0; // B
        active_pins[2] = 0; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 0; // G
        break;

      case 13: // 0xD
        active_pins[0] = 0; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 0; // F
        active_pins[6] = 1; // G
        break;

      case 14: // 0xE
        active_pins[0] = 1; // A
        active_pins[1] = 0; // B
        active_pins[2] = 0; // C
        active_pins[3] = 1; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      case 15: // 0xF
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 0; // C
        active_pins[3] = 0; // D
        active_pins[4] = 1; // E
        active_pins[5] = 1; // F
        active_pins[6] = 1; // G
        break;

      default: // backwards E for error?
        active_pins[0] = 1; // A
        active_pins[1] = 1; // B
        active_pins[2] = 1; // C
        active_pins[3] = 1; // D
        active_pins[4] = 0; // E
        active_pins[5] = 0; // F
        active_pins[6] = 1; // G
        break;
    }
    for (uint8_t pin=0; pin<7; pin++){
      digitalWrite(cur_seg_pins[pin], active_pins[pin]);
    }
  }
}

void displayOff(uint8_t *tens_pins, uint8_t *ones_pins){
      for (uint8_t pin=0; pin<7; pin++){
        digitalWrite(tens_pins[pin], 0);
        digitalWrite(ones_pins[pin], 0);
    }
}

void dispense(uint16_t ms){
  servo.attach(SERVO);
  unsigned long start_time = millis();
  unsigned long curr_time = start_time;
  uint8_t up = 0, down = 0;
  servo.write(SERVO_ON);
  while (curr_time - ms < start_time){
    curr_time = millis();
    up = !(digitalRead(UP_BTN)); //flip bit (idle = high)
    down = !(digitalRead(DOWN_BTN));
    if (up || down) { //User can hit oz up or down button to immediately stop dispensing
      servo.write(SERVO_OFF);
      delay(300);
      servo.detach();
    }
  }

  servo.write(SERVO_OFF);
  delay(300);
  servo.detach();
}

void loop() {
  uint8_t up = !(digitalRead(UP_BTN)); //flip bit (idle = high)
  uint8_t down = !(digitalRead(DOWN_BTN));
  uint8_t start = !(digitalRead(START_BTN));
  uint8_t both = up && down;
  uint16_t on_time = 0;
  static unsigned long last_interaction_time, idle = 0;
  static uint8_t num_oz = 0;

  static uint16_t pos = 1500;
  constexpr uint16_t MAX_POS = 2000; // Servo maps 1000 - 2000 uS

  if (up && down){
     num_oz = 12;
     setDisplay(num_oz, tens_pins, ones_pins);
     last_interaction_time = millis();
  }
  else if (up) {
     num_oz += 1;
     setDisplay(num_oz, tens_pins, ones_pins);
     last_interaction_time = millis();
  }
  else if (down) {
     num_oz -= 1;
     setDisplay(num_oz, tens_pins, ones_pins);
     last_interaction_time = millis();
  }
  else if (start){
    setDisplay(num_oz, tens_pins, ones_pins);    
    if (num_oz > 0 && num_oz < 30){
      on_time = num_oz * MSECS_PER_OZ;
      dispense(on_time);
    }
    last_interaction_time = millis();
  }
  else {
    idle = millis() - last_interaction_time;
    if (idle < IDLE_TIMEOUT_MS) {
      setDisplay(num_oz, tens_pins, ones_pins);
    }
    else {
      displayOff(tens_pins, ones_pins);
      delay(500);
    }
  }

  delay(DELAYMS);
}
