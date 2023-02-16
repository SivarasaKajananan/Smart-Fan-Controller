#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>

// Replace these with your network credentials
#define WIFI_SSID "Redmi9"
#define WIFI_PASSWORD "*******"

// Replace these with your Firebase project credentials
#define FIREBASE_HOST "##########"
#define FIREBASE_AUTH "**********"

#define DHTPIN D2    // Pin connected to the DHT11 sensor
#define DHTTYPE DHT11   // DHT sensor type

DHT dht(DHTPIN, DHTTYPE);   // Create a DHT object

const int ledPin1 = D3;  // Pin connected to the LED
const int ledPin2 = D4;

// Firebase Data object
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);   // Initialize serial communication
  dht.begin();            // Initialize the DHT sensor

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);  // Turn off the LED initially
  digitalWrite(ledPin2, LOW);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float temperature = dht.readTemperature();  // Read temperature from the DHT sensor

  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }

  int temp = temperature;
  Serial.println(temp);

  int fanSpeed = 1;
  // Control fan speed based on temperature
  if (temp <= 30) {
    fanSpeed = 1;
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin1, LOW);  // Set fan speed to the lowest level
  } else if (30 < temp && temp <= 32) {
    fanSpeed = 2;
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin1, LOW);
  } else if (32 < temp && temp <= 34) {
    fanSpeed = 3;
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin1, HIGH);
  } else if (34 < temp && temp <= 36) {
    fanSpeed = 4;
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin1, HIGH);
  } else {
    fanSpeed = 5; // Assuming this is the max speed
    Serial.println("max");
  }

  // Print the fan speed to the serial monitor
  Serial.print("Fan Speed: ");
  Serial.println(fanSpeed);

  // Send fan speed to Firebase
  if (Firebase.setInt(firebaseData, "/fanSpeed", fanSpeed)) {
    Serial.println("Data sent to Firebase successfully");
  } else {
    Serial.print("Failed to send data to Firebase: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(1000);  // Wait for 1 second before taking the next temperature reading
}
