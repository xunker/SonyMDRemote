#define REMOTE_DATA_PIN 3

volatile boolean pinLow = false;
void dataPinLow() {
  pinLow = true;
}

void setup() {
  // pinMode(REMOTE_DATA_PIN, INPUT_PULLUP);
  pinMode(REMOTE_DATA_PIN, INPUT);

  // attachInterrupt(digitalPinToInterrupt(REMOTE_DATA_PIN), dataPinChanged, CHANGE);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
//
//
  // attachInterrupt(digitalPinToInterrupt(REMOTE_DATA_PIN), dataPinLow, LOW);

  Serial.begin(115200);
  Serial.println("Ready");
}

unsigned long nextTrigger = 0;
#define TRIGGER_EVERY 1000 // milliseconds
uint8_t triggers = 0;
uint8_t totalTriggers = 0;

void loop() {
  if (digitalRead(REMOTE_DATA_PIN) == LOW) {
  // if (pinLow) {
    // noInterrupts();
    detachInterrupt(digitalPinToInterrupt(REMOTE_DATA_PIN));

    pinLow = false;
    digitalWrite(LED_BUILTIN, HIGH);
    unsigned long pulseLowMicros = pulseIn(REMOTE_DATA_PIN, HIGH);
    // unsigned long pulseLowMicros = pulseIn(REMOTE_DATA_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);

    // interrupts();
    attachInterrupt(digitalPinToInterrupt(REMOTE_DATA_PIN), dataPinLow, FALLING);

    totalTriggers++;
    // if (pulseLowMicros >= 150) {
    //   triggers++;
    //   // Serial.print(millis());
    //   // Serial.print(" ");
    //   Serial.print(pulseLowMicros);
    //   Serial.print(" ");
    // }

    // LOOP with pulsein between waiting for high and waiting for low
    unsigned long pulseHighMicros = pulseIn(REMOTE_DATA_PIN, LOW);

    // Serial.print(pulseLowMicros);
    // Serial.print("-");
    // Serial.print(pulseHighMicros);
    // Serial.print(" ");

  //   if (pulseHighMicros > 50000) {
  //     Serial.print(pulseLowMicros);
  //     Serial.print("-");
  //     Serial.print(pulseHighMicros);
  //     Serial.print(" ");
  //   } else {
  //     Serial.print(".");
  //   }
  // }

    if (pulseHighMicros > 1000) {
      Serial.println(pulseHighMicros);
      triggers++;
    }

    if (millis() >= nextTrigger) {
      nextTrigger = millis() + TRIGGER_EVERY;
      Serial.print("T: ");
      Serial.print(triggers);
      Serial.print(" ");
      Serial.println(totalTriggers);

      triggers = 0;
      totalTriggers = 0;
    }
  }
}
