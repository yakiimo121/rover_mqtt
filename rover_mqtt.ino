#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with HIGHs suitable for your network.

const char* ssid = "***";
const char* password = "***";
const char* mqtt_server = "***";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    // forward
    digitalWrite(4,0); 
    digitalWrite(5,0); 
    digitalWrite(12, HIGH); 
    digitalWrite(13, HIGH);
  } else if ((char)payload[0] == '2') {
    // turn right
    digitalWrite(4,0); 
    digitalWrite(5,HIGH); 
    digitalWrite(12,0); 
    digitalWrite(13,HIGH);  
  } else if ((char)payload[0] == '3') {
    // turn left
    digitalWrite(4,HIGH); 
    digitalWrite(5,0); 
    digitalWrite(12,HIGH); 
    digitalWrite(13,0);  
  } else if ((char)payload[0] == '4') {
    // backward
    digitalWrite(4,HIGH); 
    digitalWrite(5,HIGH); 
    digitalWrite(12,0); 
    digitalWrite(13,0);  
  } else if ((char)payload[0] == '5') {
    // stop
    digitalWrite(4,0); 
    digitalWrite(5,0); 
    digitalWrite(12,0); 
    digitalWrite(13,0);  
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("/rover/001");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(4,0); 
  digitalWrite(5,0); 
  digitalWrite(12,0); 
  digitalWrite(13,0);  

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}