const int fslpSenseLine = A0;
const int fslpDriveLine1 = 11;
const int fslpDriveLine2 = A1;
const int fslpBotR0 = 10;

void setup()
{
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  delay(250);
}

void loop()
{
  int pressure, position;
  pressure = fslpGetPressure();
  if (pressure == 0) {
    position = 0;
  }
  else {
    position = fslpGetPosition();
  }
  position /= 10;
  
  Serial.write(position & 0b01111111);
  Serial.write(pressure | 0b10000000);
//  char report[80];
//  sprintf(report, "pressure: %5d   position: %5d\n", pressure, position);
//  Serial.print(report);
//  Serial.println(position & 0b01111111, BIN);
//  Serial.println(pressure | 0b10000000, BIN);

  delay(20);
}

int fslpGetPosition()
{
  pinMode(fslpSenseLine, OUTPUT);
  digitalWrite(fslpSenseLine, LOW);
  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, LOW);
  pinMode(fslpDriveLine2, OUTPUT);
  digitalWrite(fslpDriveLine2, LOW);
  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);
  digitalWrite(fslpDriveLine1, HIGH);
  pinMode(fslpBotR0, INPUT);
  pinMode(fslpSenseLine, INPUT);
  delayMicroseconds(10);
  analogReset();
  return analogRead(fslpSenseLine);
}

int fslpGetPressure()
{
  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, HIGH);
  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);
  pinMode(fslpSenseLine, INPUT);
  pinMode(fslpDriveLine2, INPUT);
  delayMicroseconds(10);
  analogReset();
  int v1 = analogRead(fslpDriveLine2);
  analogReset();
  int v2 = analogRead(fslpSenseLine);
  if (v1 == v2) {
    return 32 * 1023;
  }
  return 32 * v2 / (v1 - v2);
}

void analogReset()
{
#if defined(ADMUX)
#if defined(ADCSRB) && defined(MUX5)
    // Code for the ATmega2560 and ATmega32U4
    ADCSRB |= (1 << MUX5);
#endif
    ADMUX = 0x1F;

    // Start the conversion and wait for it to finish.
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
#endif
}

