#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 5  // Connect DHT11 to TX2 (GPIO 17)
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Structure to hold sensor data
typedef struct struct_message {
  int id;
  int temperature;
  int humidity;
  int ldrValue;
} struct_message;

// Create a struct_message variable
struct_message sensorData;

// Define the MAC address of the Master Node
uint8_t masterNodeMAC[] = {0x2C, 0xBC, 0xBB, 0x0C, 0x3A, 0x50};  // MAC address of the Master Node

void setup() {
  Serial.begin(115200);
  dht.begin();
  
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

  delay(2000); // Allow sensor to stabilize
}

void sendData() {
  sensorData.id = 1;  // Set Node ID to 1 for DHT Node
  sensorData.temperature = dht.readTemperature();
  sensorData.humidity = dht.readHumidity();
  
  if (isnan(sensorData.temperature) || isnan(sensorData.humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send data to the Master Node
  esp_now_send(masterNodeMAC, (uint8_t *) &sensorData, sizeof(sensorData));
  Serial.println("Data sent to Master Node");
}

void loop() {
  sendData();  // Send data every 1 seconds
  sensorData.temperature = dht.readTemperature();
  sensorData.humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(sensorData.temperature);
  Serial.print("Humidity: ");
  Serial.println(sensorData.humidity);
  delay(5000);
}
