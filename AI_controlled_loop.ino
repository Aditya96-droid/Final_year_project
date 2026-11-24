const int GATE = 7;
const float VREF = 5.09;
const float RLOAD = 426.0;

const int N = 240;
const long SAMPLE_US = 2500; 
const int PRE = 8;        
const int ON_SAMPLES = 160; 

uint16_t adcCap[N];
uint16_t adcDrain[N];

void setup() {
  pinMode(GATE, OUTPUT);
  digitalWrite(GATE, LOW);
  Serial.begin(115200);
  Serial.println("SYSTEM_READY");
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "FIRE") {
      executePulse();
    } else if (cmd == "STATUS") {
      float v = analogRead(A0) * VREF / 1023.0;
      // Send as a single, easily parsable line
      Serial.print("STATUS_OK,");
      Serial.println(v, 3);
    }
  }
}

void executePulse() {
  delay(100); // Brief pause to ensure serial buffer is clear
  
  unsigned long nextTime = micros();
  for (int i = 0; i < N; i++) {
    while ((long)(micros() - nextTime) < 0) { }
    nextTime += SAMPLE_US;

    if (i == PRE) digitalWrite(GATE, HIGH);
    if (i == PRE + ON_SAMPLES) digitalWrite(GATE, LOW);

    analogRead(A0); // Dummy read for stability
    adcCap[i] = analogRead(A0);
    analogRead(A1);
    adcDrain[i] = analogRead(A1);
  }
  digitalWrite(GATE, LOW);

  // Stream data back to Jupyter
  Serial.println("START");
  Serial.println("t_ms,gate,vcap,vdrain,i_mA");
  for (int i = 0; i < N; i++) {
    float tMs = i * SAMPLE_US / 1000.0;
    int gate = (i >= PRE && i < PRE + ON_SAMPLES) ? 1 : 0;
    float vcap = adcCap[i] * VREF / 1023.0;
    float vdrain = adcDrain[i] * VREF / 1023.0;
    float iMA = (vcap - vdrain) / RLOAD * 1000.0;

    Serial.print(tMs, 1); Serial.print(',');
    Serial.print(gate); Serial.print(',');
    Serial.print(vcap, 3); Serial.print(',');
    Serial.print(vdrain, 3); Serial.print(',');
    Serial.println(iMA, 2);
  }
  Serial.println("END");
}