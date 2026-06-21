#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
// ================= WIFI =================
const char* ssid = "Wokwi-GUEST";
const char* password = "";
// ================= THINGSPEAK =================
unsigned long channelID = 3389675;
const char* writeAPIKey = "VBAAN7RVI738EUGO";
WiFiClient client;
// ================= DHT22 =================
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// ================= SETUP =================
void setup() {
 Serial.begin(115200);
 // Start DHT Sensor
 dht.begin();
 // Connect WiFi
 WiFi.begin(ssid, password);
 Serial.print("Connecting to WiFi");
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println();
 Serial.println("WiFi Connected!");
 // Start ThingSpeak
 ThingSpeak.begin(client);
}
// ================= LOOP =================
void loop() {
 // Read Sensor Values
 float temperature = dht.readTemperature();
float humidity = dht.readHumidity();
 // Print Serial Monitor
 Serial.println("===== WEATHER DATA =====");
 Serial.print("Temperature: ");
 Serial.print(temperature);
 Serial.println(" C");
 Serial.print("Humidity: ");
 Serial.print(humidity);
 Serial.println(" %");
 // Send Data to ThingSpeak
 ThingSpeak.setField(1, temperature);
 ThingSpeak.setField(2, humidity);
 int status = ThingSpeak.writeFields(channelID, writeAPIKey);
 if (status == 200) {
 Serial.println("Data sent successfully!");
 } else {
 Serial.print("ThingSpeak Error: ");
 Serial.println(status);
 }
 Serial.println("========================");
delay(15000);
}
