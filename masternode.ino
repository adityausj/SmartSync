#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <esp_now.h>

// WiFi and AWS credentials
const char* ssid = ""; //enter your wifi ssid
const char* password = ""; // eneter your password 
const char* awsEndpoint = "";// Enter AWS endpoint

// AWS IoT device credentials that is provide by AWS when you generate certificated
const char* rootCA =  R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               
const char* certificate = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE----- 
)KEY";
 
// Device Private Key                                               
const char* privateKey = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY----- 
)KEY";
// MQTT topic
const char* publishTopic = "ESP_master/data";

// Data structures
typedef struct struct_message {
  int id;
  int temperature;
  int humidity;
  int LdrValue;
} struct_message;

struct_message sensorData;

struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2};

WiFiClientSecure net;
PubSubClient client(net);

// Callback for ESP-NOW data reception
void OnDataRecv(const esp_now_recv_info *recvInfo, const uint8_t *incomingData, int len) {
  // Extract MAC address from recvInfo
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
           recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);
  
  Serial.print("Packet received from: ");
  Serial.println(macStr);
  
  // Copy incoming data to `sensorData`
  memcpy(&sensorData, incomingData, sizeof(sensorData));
  
  Serial.printf("Received data: ID=%d, Temperature=%d, Humidity=%d, LDR Value=%d\n",
                sensorData.id, sensorData.temperature, sensorData.humidity, sensorData.LdrValue);
  
  // Update the corresponding board structure
  if (sensorData.id == 1) {
    boardsStruct[sensorData.id - 1].temperature = sensorData.temperature;
    boardsStruct[sensorData.id - 1].humidity = sensorData.humidity;
    Serial.printf("Updated Board 1 -> Temperature: %d, Humidity: %d\n",
                  boardsStruct[0].temperature, boardsStruct[0].humidity);
  } else if (sensorData.id == 2) {
    boardsStruct[sensorData.id - 1].LdrValue = sensorData.LdrValue;
    Serial.printf("Updated Board 2 -> LDR Value: %d\n", boardsStruct[1].LdrValue);
  }
  
  // Publish to AWS IoT
  publishToAWS(sensorData);
}

// Function to connect to WiFi
void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

// Function to connect to AWS IoT
void connectAWS() {
  net.setCACert(rootCA);
  net.setCertificate(certificate);
  net.setPrivateKey(privateKey);
  client.setServer(awsEndpoint, 8883);

  while (!client.connected()) {
    Serial.print("Connecting to AWS IoT...");
    if (client.connect("ESP32Master")) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, status: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// Function to publish data to AWS IoT
void publishToAWS(struct_message data) {
  if (!client.connected()) {
    connectAWS();
  }

  String payload = "{";
  payload += "\"id\": " + String(data.id) + ",";
  payload += "\"temperature\": " + String(data.temperature) + ",";
  payload += "\"humidity\": " + String(data.humidity) + ",";
  payload += "\"LdrValue\": " + String(data.LdrValue);
  payload += "}";

  if (client.publish(publishTopic, payload.c_str())) {
    Serial.println("Data published to AWS IoT: " + payload);
  } else {
    Serial.println("Publish failed!");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  connectWiFi();
  connectAWS();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW Initialized and ready to receive data.");
}

void loop() {
  client.loop();  // Maintain AWS IoT connection
}

































