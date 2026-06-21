#include <Servo.h>

Servo gate;

// Pin definitions
const int pingPin = 7;      // HC-SR04 trigger and echo pin
const int servoPin = 9;     // Servo motor pin
const int tempPin = A0;     // LM35 output pin
const int buzzerPin = 8;    // Buzzer pin

long duration;
int distance;
float tempC;

void setup()
{
  Serial.begin(9600);

  gate.attach(servoPin);
  gate.write(0);              // Gate initially closed

  pinMode(pingPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.println("Smart Gate and Temperature Monitoring System");
}

void loop()
{
  // ===== TEMPERATURE SENSOR =====
  int sensorValue = analogRead(tempPin);

  // Convert LM35 reading to Celsius
  float voltage = sensorValue * (5.0 / 1023.0);
  tempC = voltage * 100;

  // ===== ULTRASONIC SENSOR =====
  pinMode(pingPin, OUTPUT);

  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);

  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);

  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);

  duration = pulseIn(pingPin, HIGH);

  distance = duration / 29 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ===== SERVO GATE CONTROL =====
  if (distance > 0 && distance < 15)
  {
    Serial.println("Object Detected");
    Serial.println("Gate Open");

    gate.write(90);      // Open gate
    delay(3000);         // Keep open for 3 seconds

    gate.write(0);       // Close gate

    Serial.println("Gate Closed");
  }

  // ===== HIGH TEMPERATURE ALARM =====
  if (tempC > 40)
  {
    tone(buzzerPin, 1000);
    Serial.println("HIGH TEMPERATURE ALERT");
  }
  else
  {
    noTone(buzzerPin);
  }

  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" C");

  Serial.println("--------------------");

  delay(500);
}
