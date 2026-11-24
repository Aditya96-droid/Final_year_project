const int GATE = 7;
const float VREF = 5.09;
const float RLOAD = 426.0;

void setup() {
  pinMode(GATE, OUTPUT);
  digitalWrite(GATE, LOW); // Ensure MOSFET is OFF at startup
  Serial.begin(115200);
  
  Serial.println("=====================================");
  Serial.println("  MOSFET TEST SYSTEM READY");
  Serial.println("  Type any character and press Enter");
  Serial.println("  to run ONE test cycle.");
  Serial.println("=====================================");
}

void loop() {
  // Wait for user to send a character from Serial Monitor
  if (Serial.available() > 0) {
    char cmd = Serial.read(); // Read the character to clear the buffer
    
    Serial.println("\n--- STARTING SINGLE TEST CYCLE ---");
    
    // ==========================================
    // STATE 1: MOSFET OFF
    // ==========================================
    digitalWrite(GATE, LOW);
    delay(100); // Let the gate fully discharge and ADC settle
    
    int r0_off = analogRead(A0);
    int r1_off = analogRead(A1);
    int r2_off = analogRead(A2);
    
    float vcap_off   = r0_off * VREF / 1023.0;
    float vdrain_off = r1_off * VREF / 1023.0;
    float vgate_off  = r2_off * VREF / 1023.0;
    float ima_off    = (vcap_off - vdrain_off) / RLOAD * 1000.0;
    
    Serial.println("STATE: OFF");
    Serial.print("  vcap   : "); Serial.println(vcap_off, 2);
    Serial.print("  vdrain : "); Serial.println(vdrain_off, 3);
    Serial.print("  vgate  : "); Serial.println(vgate_off, 2);
    Serial.print("  i_mA   : "); Serial.println(ima_off, 2);
    Serial.println("-----------------------------------");
    
    // ==========================================
    // STATE 2: MOSFET ON
    // ==========================================
    digitalWrite(GATE, HIGH);
    delay(100); // Let the gate fully charge and ADC settle
    
    int r0_on = analogRead(A0);
    int r1_on = analogRead(A1);
    int r2_on = analogRead(A2);
    
    float vcap_on   = r0_on * VREF / 1023.0;
    float vdrain_on = r1_on * VREF / 1023.0;
    float vgate_on  = r2_on * VREF / 1023.0;
    float ima_on    = (vcap_on - vdrain_on) / RLOAD * 1000.0;
    
    Serial.println("STATE: ON");
    Serial.print("  vcap   : "); Serial.println(vcap_on, 2);
    Serial.print("  vdrain : "); Serial.println(vdrain_on, 3);
    Serial.print("  vgate  : "); Serial.println(vgate_on, 2);
    Serial.print("  i_mA   : "); Serial.println(ima_on, 2);
    Serial.println("-----------------------------------");
    
    // ==========================================
    // STATE 3: SAFETY OFF
    // ==========================================
    digitalWrite(GATE, LOW);
    
    Serial.println("--- CYCLE COMPLETE ---");
    Serial.println("Type any character and press Enter to test again.\n");
  }
}