#include <WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
// ================= WIFI =================
const char* ssid = "Wokwi-GUEST";
const char* password = "";
WiFiClient client;
// ================= THINGSPEAK =================
unsigned long channelID = 3391651;
const char* writeAPIKey = "WOQDKA4Q978R28AU";
// ================= DHT22 =================
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// ================= PULSE SENSOR =================
#define PULSE_PIN 34
// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);
// ================= SETUP =================
void setup() {
Serial.begin(115200);
// I2C Start
Wire.begin(21, 22);
// DHT Start
dht.begin();
// LCD Start
lcd.init();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("IoT Health");
lcd.setCursor(0,1);
lcd.print("Connecting...");
// WiFi Connect
WiFi.begin(ssid, password);
Serial.print("Connecting WiFi");
while(WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println();
Serial.println("WiFi Connected!");
// ThingSpeak Start
ThingSpeak.begin(client);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("WiFi Connected");
delay(2000);
}
// ================= LOOP =================
void loop() {
// Read Temperature
float temp = dht.readTemperature();
// Read Pulse Value
int pulseValue = analogRead(PULSE_PIN);
// Convert to BPM
int bpm = map(pulseValue, 0, 4095, 60, 120);
// ===== SERIAL MONITOR =====
Serial.println("===== HEALTH DATA =====");
Serial.print("Temperature: ");
Serial.print(temp);
Serial.println(" C");
Serial.print("Heart Rate: ");
Serial.print(bpm);
Serial.println(" BPM");
// ===== LCD DISPLAY =====
lcd.clear();
lcd.setCursor(0,0);
lcd.print("T:");
lcd.print(temp);
lcd.print((char)223);
lcd.print("C");
lcd.setCursor(0,1);
lcd.print("BPM:");
lcd.print(bpm);
// ===== SEND DATA TO THINGSPEAK =====
ThingSpeak.setField(1, temp);
ThingSpeak.setField(2, bpm);
int status = ThingSpeak.writeFields(channelID, writeAPIKey);
if(status == 200) {
Serial.println("Data sent successfully");
}
else {
Serial.print("ThingSpeak Error: ");
Serial.println(status);
}
delay(15000);
}
