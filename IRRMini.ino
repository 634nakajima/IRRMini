int irr = 3;
byte data;
byte mColor = 0x00;
byte signal = 0x00;
byte pirr;

unsigned long time = 0;
unsigned long dt = 0;
unsigned long timeout = 0;
unsigned long t1 = 0;
unsigned long dt1 = 0;

void setup() {
  pinMode(irr, INPUT);

  pirr = 0x00;
  time = micros();
  
  Serial.begin(9600);
}

void loop() {

  if(detectLeader(irr)) {
    data = mColor;
    Serial.write(data);
    pirr = data;
  }else {
    Serial.write((byte)0);
  }
  delay(100);
}

boolean detectLeader(const int irr) {
  time = micros();
  while(!digitalRead(irr));
  dt = micros() - time;
  timeout += dt;
  
  if (dt > 8000 && dt < 9000) {
    time = micros();
    while(digitalRead(irr));
    dt = micros() - time;
    timeout += dt;
    
    if (dt > 4000 && dt < 5000) {
      timeout = 0;
      return true;
    }
  }
  
  if (timeout > 60000) {
    timeout = 0;
    return false;
  }else {
    detectLeader(irr);
  }
}

byte analyzeSignal(const int irr) {
  while(!digitalRead(irr));
  time = micros();
  while(digitalRead(irr));
  dt = micros() - time;
  
  if(dt > 500 && dt < 700) {
    return 0x00 | analyzeSignal(irr) << 1;
  }else if(dt > 1600 && dt < 1800) {
    return 0x01 | analyzeSignal(irr) << 1;
  }else if(dt > 10000) {
    return signal;
  }
}
