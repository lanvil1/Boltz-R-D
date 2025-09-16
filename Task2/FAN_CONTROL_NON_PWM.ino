// Thermistor parameters from the datasheet
#define RT0 10000
#define B 3977
#define R 10000  
#define T0 298.15

// Fan control parameters
unsigned long fanDelay = 0;
int fanDelayLength = 20;
unsigned long pulseDelay = 0;
int pulseLength1 = 0;      // 0% duty cycle
int pulseLength2 = 10000;  // 50% duty cycle
int pulseLength3 = 20000;  // 100% duty cycle

float RT, VR, ln, TC, VRT;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  fanDelay = millis();
}

void updatePWM0() {
  if (millis() > fanDelay + fanDelayLength) {
    fanDelay += fanDelayLength;
    digitalWrite(13, HIGH);
    pulseDelay = micros();
  }
  if (micros() > pulseDelay + pulseLength1) {
    pulseDelay += pulseLength1;
    digitalWrite(13, LOW);
  }
}

void updatePWM50() {
  if (millis() > fanDelay + fanDelayLength) {
    fanDelay += fanDelayLength;
    digitalWrite(13, HIGH);
    pulseDelay = micros();
  }
  if (micros() > pulseDelay + pulseLength2) {
    pulseDelay += pulseLength2;
    digitalWrite(13, LOW);
  }
}

void updatePWM100() {
  if (millis() > fanDelay + fanDelayLength) {
    fanDelay += fanDelayLength;
    digitalWrite(13, HIGH);
    pulseDelay = micros();
  }
  if (micros() > pulseDelay + pulseLength3) {
    pulseDelay += pulseLength3;
    digitalWrite(13, LOW);
  }
}

void loop() {

  VRT = (5.00 / 1023.00) * analogRead(A0);
  VR = 5.00 - VRT;
  RT = VRT / (VR / R);
  
  ln = log(RT / RT0);
  TC = (1 / ((ln / B) + (1 / T0)));
  TC = TC - 273.15; 
  

  Serial.print("Temperature: ");
  Serial.print(TC);
  Serial.print("C\tFan State: ");
  

  if (TC < 25) {
    updatePWM0();
    Serial.println("OFF");
  } else if (TC >= 25 && TC < 30) {
    updatePWM50();
    Serial.println("50%");
  } else {
    updatePWM100();
    Serial.println("100%");
  }
  
  delay(500);
}
