#define NO_SWITCH_PRESSED         0
#define SWITCH_IN                 2
#define SWITCH_OUT                3
#define SWITCH_IN_ENABLE_BUTTON   6
#define SWITCH_OUT_ENABLE_BUTTON  7
#define SWITCH_IN_LED             8
#define SWITCH_OUT_LED            9

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

ISR(TIMER1_COMPA_vect)
{
  if (currentState == PUMP_OPEN_STATE) {
    pumpOpenTimeCounter++;
  } else if (currentState == PUMP_DISABLED_STATE) {
    pumpDisabledTimeCounter++;
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

void pollConfiguration() {
  
}
