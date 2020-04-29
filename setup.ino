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
  OCR1A = TIMER_SETUP_VALUE;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  interrupts();

  Serial.begin(115200);
  while (!Serial) {
    ;
  }
}
