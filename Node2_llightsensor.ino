#include <esp_now.h>
#include <WiFi.h>

#define LDR_PIN 34   // Analog pin connected to LDR
#define LED_PIN 26   // GPIO pin connected to LED

const int threshold = 30;  // Threshold for darkness (adjust based on LDR values)

typedef struct struct_message {
  int id;
  int temperature;
  int humidity;
  int LdrValue;
}struct_message;

// Create a struct_message variable
struct_message sensorData;

// Define the MAC address of the Master Node
uint8_t masterNodeMAC[] = {};  // MAC address of the Master Node

void setup() {
  Serial.begin(115200);
  // Initialize WiFi in STA mode
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, masterNodeMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Configure the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Ensure LED is initially off

  // Confirm setup
  Serial.println("LDR Darkness Detector Initialized");
  delay(2000);
}

void sendData() {
  sensorData.id = 2;  // Set Node ID to 2 for Photoresistor Node
  sensorData.LdrValue = analogRead(LDR_PIN);

  // Send data to the Master Node
  esp_now_send(masterNodeMAC, (uint8_t *) &sensorData, sizeof(sensorData));
  Serial.println("Data sent to Master Node");
}

void loop() {
  sendData();
  // Read LDR value
  int LdrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(LdrValue);

  // Check for darkness
  if (LdrValue > threshold) {
    // Darkness detected
    Serial.println("Darkness detected. Turning ON the LED.");
    digitalWrite(LED_PIN, HIGH);  // Turn on the LED
  } else {
    // Bright light
    Serial.println("Light detected. Turning OFF the LED.");
    digitalWrite(LED_PIN, LOW);  // Turn off the LED
  }

  // Small delay for stability
  delay(1000);
}

