#define PUMP                      4
#define LEDS                      5

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
