void setup() {
  pinMode(PUMP, OUTPUT);
  pinMode(FANS, OUTPUT);
  pinMode(SWITCH_IN, INPUT_PULLUP);
  pinMode(SWITCH_OUT, INPUT_PULLUP);
  pinMode(SWITCH_IN_ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(SWITCH_OUT_ENABLE_BUTTON, INPUT_PULLUP);
  pinMode(SWITCH_IN_LED, OUTPUT);
  pinMode(SWITCH_OUT_LED, OUTPUT);
  digitalWrite(PUMP, LOW);
  digitalWrite(FANS, LOW);
  attachInterrupt(digitalPinToInterrupt(SWITCH_IN), switchInPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_OUT), switchOutPressed, FALLING);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = TIMER_SETUP_VALUE;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TCCR2A  = 0;
  TCCR2B  = 0;
  TCNT2   = 0;
  OCR2A   = 255;
  TCCR2B |= (1 << WGM12);   // CTC mode
  TCCR2B |= (1 << CS12);    // 256 prescaler
  interrupts();

//  Serial.begin(115200);
//  while (!Serial) {
//    ;
//  }
//  Serial.println("asdfasdfa");
}
