#define NO_SWITCH_PRESSED     0
#define SWITCH_IN             2
#define SWITCH_OUT            3
#define PUMP                  13
#define LEDS                  5

#define DEBOUNCE_DELAY        100     // Miliseconds
#define PUMP_OPEN_TIME        7       // Seconds
#define PUMP_DISABLED_TIME    5       // Seconds
#define IDLE_STATE            0
#define PUMP_OPEN_STATE       1
#define PUMP_DISABLED_STATE   2

unsigned int timerValue = 62500;       // 16MHz-256-1Hz
unsigned int timerCount = 0;
unsigned int pumpOpenTimeCounter = 0;
unsigned int pumpDisabledTimeCounter = 0;
unsigned char currentState = IDLE_STATE;
unsigned char switchPreviouslyPressed = NO_SWITCH_PRESSED;
unsigned char bothWaysEnabled = 0;

void setup() {
  pinMode(PUMP, OUTPUT);
  pinMode(LEDS, OUTPUT);
  pinMode(SWITCH_IN, INPUT_PULLUP);
  pinMode(SWITCH_OUT, INPUT_PULLUP);
  digitalWrite(PUMP, LOW);
  digitalWrite(LEDS, HIGH);
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
    ;
  }
}

void switchInPressed() {
  noInterrupts();
  delay(DEBOUNCE_DELAY);
  if (digitalRead(SWITCH_IN) == LOW) {
    if (bothWaysEnabled == 1) {
      startTimer();
      currentState = PUMP_OPEN_STATE;
    } else {
      if(switchPreviouslyPressed == NO_SWITCH_PRESSED || switchPreviouslyPressed == SWITCH_IN) {
        startTimer();
        switchPreviouslyPressed = SWITCH_IN;
        currentState = PUMP_OPEN_STATE;
      }
    }
  }
  interrupts();
}

void switchOutPressed() {
  noInterrupts();
  delay(DEBOUNCE_DELAY);
  if (digitalRead(SWITCH_OUT) == LOW) {
    if (bothWaysEnabled == 1) {
      startTimer();
      currentState = PUMP_OPEN_STATE;
    } else {
      if(switchPreviouslyPressed == NO_SWITCH_PRESSED || switchPreviouslyPressed == SWITCH_OUT) {
        startTimer();
        switchPreviouslyPressed = SWITCH_OUT;
        currentState = PUMP_DISABLED_STATE;
      }
    }
  }
  interrupts();
}

void startTimer() {
  TCNT1  = 0;
  TIMSK1 |= (1 << OCIE1A);
  pumpOpenTimeCounter = 0;
  pumpDisabledTimeCounter = 0;
}

void stopTimer() {
  TCNT1  = 0;
  TIMSK1 &= ~(1 << OCIE1A);
  pumpOpenTimeCounter = 0;
  pumpDisabledTimeCounter = 0;
}

void turnOnPump() {
  if (digitalRead(PUMP) != HIGH) {
    digitalWrite(PUMP, HIGH);
    digitalWrite(LEDS, LOW);
  }
}

void turnOffPump() {
  if (digitalRead(PUMP) != LOW) {
    digitalWrite(PUMP, LOW);
    digitalWrite(LEDS, HIGH);
  }
}

ISR(TIMER1_COMPA_vect)
{
  if (currentState == PUMP_OPEN_STATE) {
    pumpOpenTimeCounter++;
  } else if (currentState == PUMP_DISABLED_STATE) {
    pumpDisabledTimeCounter++;
  }
}

void loop() {
  if (currentState == IDLE_STATE) {
    turnOffPump();
  } 
  else if (currentState == PUMP_OPEN_STATE) {
    if (pumpOpenTimeCounter >= PUMP_OPEN_TIME) {
      stopTimer();
      switchPreviouslyPressed = NO_SWITCH_PRESSED;
      currentState = IDLE_STATE;
    } else {
      turnOnPump();
    }
  }
  else if (currentState == PUMP_DISABLED_STATE) {
    if (pumpDisabledTimeCounter >= PUMP_DISABLED_TIME) {
      stopTimer();
      switchPreviouslyPressed = NO_SWITCH_PRESSED;
      currentState = IDLE_STATE;
    } else {
      turnOffPump();
    }
  }
  delay(1);
}
