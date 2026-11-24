const int   GATE       = 7;
const float VREF       = 5.09;
const float RLOAD      = 426.0;    // your measured load resistor

const int   N          = 240;
const long  SAMPLE_US  = 2500;     // 2.5 ms between samples
const int   PRE        = 8;        // samples recorded before the MOSFET turns on
const int   ON_SAMPLES = 160;      // MOSFET on for 400 ms

uint16_t adcCap[N];
uint16_t adcDrain[N];

void setup() {
  pinMode(GATE, OUTPUT);
  digitalWrite(GATE, LOW);
  Serial.begin(115200);
  Serial.println("Ready. Type g and press Enter to fire one pulse.");
}

void loop() {
  if (Serial.available() == 0) return;
  delay(50);
  while (Serial.available() > 0) Serial.read();

  delay(1500);                                     // let the capacitor charge fully

  unsigned long nextTime = micros();
  for (int i = 0; i < N; i++) {
    while ((long)(micros() - nextTime) < 0) { }
    nextTime += SAMPLE_US;

    if (i == PRE)              digitalWrite(GATE, HIGH);
    if (i == PRE + ON_SAMPLES) digitalWrite(GATE, LOW);

    analogRead(A0);
    adcCap[i]   = analogRead(A0);
    analogRead(A1);
    adcDrain[i] = analogRead(A1);
  }
  digitalWrite(GATE, LOW);

  Serial.println("START");
  Serial.println("t_ms,gate,vcap,vdrain,i_mA");
  for (int i = 0; i < N; i++) {
    float tMs    = i * SAMPLE_US / 1000.0;
    int   gate   = (i >= PRE && i < PRE + ON_SAMPLES) ? 1 : 0;
    float vcap   = adcCap[i]   * VREF / 1023.0;
    float vdrain = adcDrain[i] * VREF / 1023.0;
    float iMA    = (vcap - vdrain) / RLOAD * 1000.0;

    Serial.print(tMs, 1);     Serial.print(',');
    Serial.print(gate);       Serial.print(',');
    Serial.print(vcap, 3);    Serial.print(',');
    Serial.print(vdrain, 3);  Serial.print(',');
    Serial.println(iMA, 2);
  }
  Serial.println("END");
}