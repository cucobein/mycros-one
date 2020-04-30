#define TIMER_SETUP_VALUE         62500   // 16MHz-256-1Hz
#define NO_SWITCH_PRESSED         0
#define CARPET_DEBOUNCE_DELAY     100     // Miliseconds
#define PUMP_OPEN_TIME            7       // Seconds
#define PUMP_DISABLED_TIME        5       // Seconds
#define IDLE_STATE                0
#define PUMP_OPEN_STATE           1
#define PUMP_DISABLED_STATE       2

unsigned int timerCount = 0;
unsigned int pumpOpenTimeCounter = 0;
unsigned int pumpDisabledTimeCounter = 0;
unsigned char currentState = IDLE_STATE;
unsigned char switchPreviouslyPressed = NO_SWITCH_PRESSED;
unsigned char bothWaysEnabled = 0;

void loop() {  
  pollConfiguration();
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
}
