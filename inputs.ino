#define NO_SWITCH_PRESSED           0
#define POLLING_DEBOUNCE_DELAY      1
#define POLLING_DEBOUNCE_THRESHOLD  30
#define SWITCH_IN                   2
#define SWITCH_OUT                  3
#define SWITCH_IN_ENABLE_BUTTON     6
#define SWITCH_OUT_ENABLE_BUTTON    7
#define SWITCH_IN_LED               8
#define SWITCH_OUT_LED              9

typedef enum configuration_state {
  NONE,
  ENTRANCE_IN,
  ENTRANCE_OUT,
  BOTH_ENTRANCES
} configuration_state;
configuration_state configuration = NONE;

typedef struct configuration_button {
  unsigned char debounceCounter;
  unsigned char state;
} configuration_button;
configuration_button switchInEnableButton = { 0, LOW };
configuration_button switchOutEnableButton = { 0, LOW };

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
  delay(CARPET_DEBOUNCE_DELAY);
  if (digitalRead(SWITCH_IN) == LOW) {
    if (bothWaysEnabled == 1) {
      startTimer();
      currentState = PUMP_OPEN_STATE;
    } else {
      if (switchPreviouslyPressed == NO_SWITCH_PRESSED || switchPreviouslyPressed == SWITCH_IN) {
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
  delay(CARPET_DEBOUNCE_DELAY);
  if (digitalRead(SWITCH_OUT) == LOW) {
    if (bothWaysEnabled == 1) {
      startTimer();
      currentState = PUMP_OPEN_STATE;
    } else {
      if (switchPreviouslyPressed == NO_SWITCH_PRESSED || switchPreviouslyPressed == SWITCH_OUT) {
        startTimer();
        switchPreviouslyPressed = SWITCH_OUT;
        currentState = PUMP_DISABLED_STATE;
      }
    }
  }
  interrupts();
}

void pollConfiguration() {
  // Debounce filter
  unsigned char switchInEnableButtonCurrentState = digitalRead(SWITCH_IN_ENABLE_BUTTON);
  unsigned char switchOutEnableButtonCurrentState = digitalRead(SWITCH_OUT_ENABLE_BUTTON);

  if (switchInEnableButton.state != switchInEnableButtonCurrentState) {
    switchInEnableButton.debounceCounter++;
  } if (switchInEnableButton.debounceCounter >= POLLING_DEBOUNCE_THRESHOLD) {
    switchInEnableButton.state = switchInEnableButtonCurrentState;
    switchInEnableButton.debounceCounter = 0;
  }

  if (switchOutEnableButton.state != switchOutEnableButtonCurrentState) {
    switchOutEnableButton.debounceCounter++;
  } if (switchOutEnableButton.debounceCounter >= POLLING_DEBOUNCE_THRESHOLD) {
    switchOutEnableButton.state = switchOutEnableButtonCurrentState;
    switchOutEnableButton.debounceCounter = 0;
  }

  // Output setup
  digitalWrite(SWITCH_IN_LED, switchInEnableButton.state);
  digitalWrite(SWITCH_OUT_LED, switchOutEnableButton.state);

  // Configuration setup
  if (switchInEnableButton.state == LOW && switchOutEnableButton.state == LOW) {
    configuration = BOTH_ENTRANCES;
  } else if (switchInEnableButton.state == LOW && switchOutEnableButton.state == HIGH) {
    configuration = ENTRANCE_IN;
  } else if (switchInEnableButton.state == HIGH && switchOutEnableButton.state == LOW) {
    configuration = ENTRANCE_OUT;
  } else {
    configuration = NONE;
  }
  delay(POLLING_DEBOUNCE_DELAY);
}
