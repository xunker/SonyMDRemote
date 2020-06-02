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

unsigned long lastMessageAt = 0;

unsigned long wentLowAt = 0;
unsigned long wentHighAt = 0;

#define FRAME_BEGIN_PULSE_MICROS 800

unsigned long lastFrameAtMicros = 0;

void loop() {
  if (wentLowAt > 0) {
    if (digitalRead(REMOTE_DATA_PIN) == HIGH) {
      wentHighAt = micros();
      digitalWrite(LED_BUILTIN, LOW);

      unsigned int lowForMicros = wentHighAt - wentLowAt;

      if (lowForMicros > FRAME_BEGIN_PULSE_MICROS) {
        // digitalWrite(LED_BUILTIN, HIGH);
        // Serial.println(lowForMicros);

        unsigned int startedWaiting = micros();

        while (digitalRead(REMOTE_DATA_PIN) == HIGH) { }
        unsigned int highForMicros = micros() - startedWaiting;

        if (highForMicros > FRAME_BEGIN_PULSE_MICROS) {
          Serial.println("");
          Serial.print(lowForMicros);
          Serial.print("-");
          Serial.print(highForMicros);
          Serial.print("(");
          Serial.print((micros() - lastFrameAtMicros)/1000);
          Serial.print(")");
          lastFrameAtMicros = wentLowAt;
        }
      } else {
        Serial.print(".");
      }
      wentLowAt = 0;
    }
  } else {
    if (digitalRead(REMOTE_DATA_PIN) == LOW) {
      wentLowAt = micros();
      wentHighAt = 0;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }


  // digitalWrite(LED_BUILTIN, LOW);
}
