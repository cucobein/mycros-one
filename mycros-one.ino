
#define SWITCH_IN 2
#define SWITCH_OUT 3
#define PUMP 13
#define DEBOUNCE_DELAY 50 // Miliseconds
#define PUMP_TIME 3 // Seconds
#define IDLE_STATE        0
#define PUMP_OPEN_STATE   1

float timerValue = 62500; // 16MHz-256-1Hz
float timerCount = 0;
float pumpTimeCounter = 0;
float currentState = IDLE_STATE;

void setup() {
  pinMode(PUMP, OUTPUT);
  pinMode(SWITCH_IN, INPUT_PULLUP);
  pinMode(SWITCH_OUT, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);
  attachInterrupt(digitalPinToInterrupt(SWITCH_IN), switchInPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_OUT), switchOutPressed, FALLING);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = timerValue;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  interrupts(); 

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
}

void switchInPressed() {
  noInterrupts();
  delay(DEBOUNCE_DELAY);
  if(digitalRead(SWITCH_IN) == LOW) {
    startTimer();
    currentState = PUMP_OPEN_STATE;
  }
  interrupts();
}

void switchOutPressed() {
  noInterrupts();
  delay(DEBOUNCE_DELAY);
  if(digitalRead(SWITCH_OUT) == LOW) {
    startTimer();
    currentState = PUMP_OPEN_STATE;
  }
  interrupts();
}

void startTimer() {
  TCNT1  = 0;
  TIMSK1 |= (1 << OCIE1A);
  pumpTimeCounter = 0;
}

void stopTimer() {
  TCNT1  = 0;
  TIMSK1 &= ~(1 << OCIE1A);
  pumpTimeCounter = 0;
}

void turnOnPump() {
  if(digitalRead(PUMP) != HIGH) {
    digitalWrite(PUMP, HIGH); 
  }
}

void turnOffPump() {
  if(digitalRead(PUMP) != LOW) {
    digitalWrite(PUMP, LOW); 
  }
}

ISR(TIMER1_COMPA_vect)                   
{
  pumpTimeCounter++;                     
}

void loop() {
  if(currentState == IDLE_STATE) {
    turnOffPump();
  } else if(currentState == PUMP_OPEN_STATE) {
    if(pumpTimeCounter >= PUMP_TIME) {
      stopTimer();
      currentState = IDLE_STATE;
    } else {
      turnOnPump();
    }
  }
}
