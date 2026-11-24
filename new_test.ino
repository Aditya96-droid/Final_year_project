const int   GATE = 7;
const float VREF = 5.09;
const int   NAVG = 100;                  // readings averaged per channel

float readAvg(int pin) {
  analogRead(pin);                       // dummy read
  long sum = 0;
  for (int i = 0; i < NAVG; i++) sum += analogRead(pin);
  return sum / (float)NAVG * VREF / 1023.0;
}

void setup() {
  pinMode(GATE, OUTPUT);
  digitalWrite(GATE, LOW);               // MOSFET off
  Serial.begin(115200);
  Serial.println("Ready. Type g and press Enter to take one measurement.");
}

void loop() {
  if (Serial.available() == 0) return;
  delay(50);
  while (Serial.available() > 0) Serial.read();

  digitalWrite(GATE, LOW);
  delay(1000);                           // capacitor fully charged
  digitalWrite(GATE, HIGH);              // MOSFET on
  delay(1500);                           // wait for the voltages to settle

  float vcap   = readAvg(A0);
  float vdrain = readAvg(A1);
  float vgate  = readAvg(A2);
  digitalWrite(GATE, LOW);               // MOSFET off again

  Serial.print("vcap = ");      Serial.print(vcap, 3);
  Serial.print("   vdrain = "); Serial.print(vdrain, 3);
  Serial.print("   vgate = ");  Serial.println(vgate, 3);
}