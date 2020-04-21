
#define SWITCH_IN 2
#define SWITCH_OUT 3
#define PUMP 13
float value = 3035; 

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
  TCNT1 = 49910;
  TCCR1B |= (1 << CS10)|(1 << CS12);  
  TIMSK1 |= (1 << TOIE1);  
  interrupts(); 
}

void switchInPressed() {
  
}

void switchOutPressed() {
  
}

ISR(TIMER1_OVF_vect)                   
{
  TCNT1 = value;                               
//  digitalWrite(ledPin, digitalRead(ledPin) ^ 1); 
}

void loop() {
}
