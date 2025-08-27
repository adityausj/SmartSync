#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <esp_now.h>

// WiFi and AWS credentials
const char* ssid = "SpectrumSetup-A517";
const char* password = "capablewindow825";
const char* awsEndpoint = "a1ftrqnxm6iffk-ats.iot.us-east-2.amazonaws.com";

// AWS IoT device credentials
const char* rootCA =  R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               
const char* certificate = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAIMafpfci2VZa3GBCD+iSTFoGugWMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDExMTgwNDU1
NDJaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC3c+csNbd4vxXNzyxJ
Nyug+Hct+egSgbBc9qMdW6/yPxXMxg8YojV4KfwycUakaJmqTN1uU5p4o/h90LTb
nIVVFTjZiq8fnrmsloGsm8sIca/kGuVaVpgMCbVSFNNks7yG+JKdLY64VP/SIfA3
jyr49ApwKxcm7ov9pull70jOFu6/7KhjywpoxFA6+4oGr4vnkIYkIBwJlTyjbnHa
weqGlH44kGD7n9lq6Z1Al7HsgJtYKK7AIyQQH63vfTvS/CpQoUEXW4IhMJyJ/Q5z
/v7jD2/TN3He9AmKYPWyyWnxBMYXbh0jMR45KCXBivM+oM3oKXGGTNtyBK50vyva
Yme1AgMBAAGjYDBeMB8GA1UdIwQYMBaAFNmS3i/yjsnFJRfYP/iCE1HLJOXaMB0G
A1UdDgQWBBQonc7t8Yle4nyqFkU3D4yleLxxYTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAMrBIvKbipoUAmEp6UnlqqAi/
0iv40YO12UmdZ1TDVZAhYEeZyL071O2cZ/lzITUg9e8uOw4dyvXP2Jx0hwh5vaHZ
sLL5jT17oIUD1+OsS/GwUDuE9lNXmVMzlquASdsX+/B7kCAAhU0s80WFuFsYXhsg
zj4qCNyz5nd6Tr3vZRYmN1ypqTpLBD2aCud/012d6YjDNILUPO5A1YaNiRXHGcf+
Zex8Ql10snAwQyxhkTHCBhH0zCLn9h4BxyuN8aFyXOUgl8kV+gK6lAV17oWx0tSE
T8XNWpzZxLQ/wCmTp9qKAM+T5hFeMc3jS1y2T5dbkcDW+3iJKkDNBVQ6s3mRFA==
-----END CERTIFICATE-----

 
 
)KEY";
 
// Device Private Key                                               
const char* privateKey = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAt3PnLDW3eL8Vzc8sSTcroPh3LfnoEoGwXPajHVuv8j8VzMYP
GKI1eCn8MnFGpGiZqkzdblOaeKP4fdC025yFVRU42YqvH565rJaBrJvLCHGv5Brl
WlaYDAm1UhTTZLO8hviSnS2OuFT/0iHwN48q+PQKcCsXJu6L/abpZe9Izhbuv+yo
Y8sKaMRQOvuKBq+L55CGJCAcCZU8o25x2sHqhpR+OJBg+5/ZaumdQJex7ICbWCiu
wCMkEB+t73070vwqUKFBF1uCITCcif0Oc/7+4w9v0zdx3vQJimD1sslp8QTGF24d
IzEeOSglwYrzPqDN6ClxhkzbcgSudL8r2mJntQIDAQABAoIBAFtaSQS33xwus3Hg
N6Jh6cTn1Zk8d/c3Q5aXbtSAwtXz2iw1eHEA3XVwGtkXlWDrgZHPYvi8gxCOPoCQ
fG9buv2KatvRdrFdPgKWlA5bKjiTPE5vokRhGKft8a/lFPtxkXD4uTGx7P8XvKhs
JA3WqkR8afI/UvEdGvL9uf37sRqGckvv1tr+wk8dUO89AUwnlIrNOdXUCSzxYqge
bdV+mK5rxvPVHJWYBh9Kd1wxNtdskHFeZCdD9Nxn9uYCJFtPD7rLWFbksA5w4tch
AGt+4/8XZ24KV//x2RPfJHt+Rg8ZgQRA1ItPPlhH/5fOeDywNhkcBbQI+WeuRBh7
zpqNYaUCgYEA7EckiQwxoqIrPZheCge0IJgx4BY2XnIqZTgVud77UDDvKq91S7n3
cyJm6qbxOsA6G6wu5W36GtfQ1yw057nQvnnRv+iJ+xbWpaI+MMFar/R/1/KVgkEn
RF7nATRivGJUaE38KSreU7JYTpcvFIl9btQYgHfY/vyCYkWYQ7U0i3sCgYEAxsP5
+5DBkOWUA0eM/8dv8v+YOJoTox3zgjnRFSS6SzFguYHXaBpUUXxFTYFKAQg+P11K
W/9Aq4Y6JqhaRAGMCieZTJeyctkc6ulAv45T9HSWDYwCddSuGUFoTXGN0R7lFFOZ
z8pzjUIZwjj88fza/kzduDVE1rg8SeNEQHY6Go8CgYADavQIV3jQVXAyr+WMV5gv
cIQoLaoTq6sZU7pcufEAs/SBtZAIzGPb8n9ynX+XzfgJNnBnOTQI9145/pW1VGbE
mpaCJCb6ztXEB5jGs8VQDkVbJlMrUy3yOz00M0HKX9HAwIAJSGl25HmKiX1Q2rxJ
wh+onBMzfQuVyFPvhst/yQKBgD6WHvTTk9+oKn8N3Jv9lIaTbL8F5QELwU1UHz3l
HrPmjGpdNBKXl81AsCHDgCXrDo6tNWjaauaXkV/yytl3Ez1vUpoHei0nqC9f2v92
NYFX5JfHak7n9RoGum0IQHuEmuVXnasMOG5WTf4WK6+Wyu4gDgCe5VonEPTDFw6l
K46JAoGBAJFMTTtB6t1LeDKGQffHfTGhbUyUxUIGPB451E2QATNfNKDhdiH05fWj
V3M98FpaGsbLXebGXF/4fulh0FyMz7Z4wNjeod9ZL6QiGBBXIoQ/X7/274rb0FyU
gcmfTEvKe9fWEWoJYzatCL8D8598G17LRjwjdgbtuQecptBUS/id
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

































