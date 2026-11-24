const int GATE = 7;
const float VREF = 5.09;
const float RLOAD = 426.0;

void setup() {
  pinMode(GATE, OUTPUT);
  digitalWrite(GATE, LOW); // SAFETY: Keep MOSFET OFF at startup
  Serial.begin(115200);
  Serial.println("System Ready. Type 't' and press Enter to capture transient curve.");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 't') {
      captureTransient();
    }
  }
}

void captureTransient() {
  // Print CSV Header for Jupyter
  Serial.println("time_ms,vcap,vdrain,vgate"); 
  
  // Turn MOSFET ON to start discharging the capacitor
  digitalWrite(GATE, HIGH); 
  unsigned long startTime = millis();
  
  // Capture data for 1500ms (1.5 seconds)
  while (millis() - startTime < 1500) {
    unsigned long t = millis() - startTime;
    
    // Read all three sensors
    float vcap   = analogRead(A0) * VREF / 1023.0;
    float vdrain = analogRead(A1) * VREF / 1023.0;
    float vgate  = analogRead(A2) * VREF / 1023.0;
    
    // Print in clean CSV format
    Serial.print(t); Serial.print(",");
    Serial.print(vcap, 3); Serial.print(",");
    Serial.print(vdrain, 3); Serial.print(",");
    Serial.println(vgate, 3);
    
    // Wait ~0.5ms to control sampling rate (~2000 samples per second)
    delayMicroseconds(500); 
  }
  
  // Turn MOSFET OFF to let capacitor recharge
  digitalWrite(GATE, LOW); 
  
  Serial.println("CAPTURE_COMPLETE");
  Serial.println("Type 't' to run again.\n");
}