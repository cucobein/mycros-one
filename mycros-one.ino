
#define SWITCH_IN         2
#define SWITCH_OUT        3
#define PUMP              13
#define DEBOUNCE_DELAY    50    // Miliseconds
#define PUMP_TIME         5     // Seconds
#define IDLE_STATE        0
#define PUMP_OPEN_STATE   1
#define N_SAMPLES         128
#define THRESHOLD_VALUE   0.5

unsigned int timerValue = 62500;       // 16MHz-256-1Hz
unsigned int timerCount = 0;
unsigned int pumpTimeCounter = 0;
unsigned char currentState = IDLE_STATE;
double inValues[N_SAMPLES];
double outValues[N_SAMPLES];
unsigned int samplesIndex = 0;

void setup() {
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);

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
    ; // wait for serial port to connect
  }
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

void sampleAnalogValues() {
  double inValue = double(analogRead(A0) * 5.0 / 1024.0);
  double outValue = double(analogRead(A1) * 5.0 / 1024.0);
  if(samplesIndex < N_SAMPLES) {
    inValues[samplesIndex] = inValue;
    outValues[samplesIndex] = outValue;
    samplesIndex++;
  } else {
    double inAverage = samplesAverage(inValues);
    double outAverage = samplesAverage(outValues);
    if(inAverage >= THRESHOLD_VALUE || outAverage >= THRESHOLD_VALUE) {
      startTimer();
      currentState = PUMP_OPEN_STATE;
    } 
    clearSamples();
  }
}

double samplesAverage(double a[]) 
{ 
    double sum = 0; 
    for (int i=0; i < N_SAMPLES; i++) 
       sum += a[i]; 
    return sum / double(N_SAMPLES); 
} 

void clearSamples() 
{ 
    for(int i=0; i<N_SAMPLES; i++) {
      inValues[i] = 0;
      outValues[i] = 0;
    }
    samplesIndex = 0;
} 

ISR(TIMER1_COMPA_vect)                   
{
  pumpTimeCounter++;                     
}

void loop() {
  sampleAnalogValues();
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
  delay(1);
}
