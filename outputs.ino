#define PUMP                      4
#define LEDS                      5
#define FANS                      10

void startPumpTimer() {
  TCNT1  = 0;
  TIMSK1 |= (1 << OCIE1A);
  pumpOpenTimeCounter = 0;
  pumpDisabledTimeCounter = 0;
}

void stopPumpTimer() {
  TCNT1  = 0;
  TIMSK1 &= ~(1 << OCIE1A);
  pumpOpenTimeCounter = 0;
  pumpDisabledTimeCounter = 0;
}

void startExtractorFans() {
  TCNT2  = 0;
  TIMSK2 |= (1 << OCIE1A);
  pumpOpenTimeCounter = 0;
  turnOnFans();
}

void stopExtractorFans() {
  TCNT2  = 0;
  TIMSK2 &= ~(1 << OCIE1A);
  pumpOpenTimeCounter = 0;
  turnOffFans();
}

void goToPumpOpenState() {
  startPumpTimer();
  startExtractorFans();
  currentState = PUMP_OPEN_STATE;
}

void goToPumpDisabledState() {
  startPumpTimer();
  currentState = PUMP_DISABLED_STATE;
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

void turnOnFans() {
  if (digitalRead(FANS) != HIGH) {
    digitalWrite(FANS, HIGH);
  }
}

void turnOffFans() {
  if (digitalRead(FANS) != LOW) {
    digitalWrite(FANS, LOW);
  }
}
