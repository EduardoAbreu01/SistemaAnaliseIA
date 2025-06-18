#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Configurações Wi-Fi
const char* ssid = "Sua rede";
const char* password = "senha da rede";

// Configurações MQTT
const char* mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_client_id = "ESP32Client";
const char* mqtt_topic = "sensor/data";

WiFiClient espClient;
PubSubClient client(espClient);

// Pinos 
const int pinTemp = 34; // Ex-A0
const int pinVolt = 35; // Ex-A1
const int potPin  = 32; // Ex-A2
const int pinFan  = 3;  // PWM para simular ventoinha
const int pinRele = 2;  // GPIO para relé

const byte RELAY_ON  = LOW;   // fecha contato
const byte RELAY_OFF = HIGH;  // abre contato

float tempC, tensao, corrente, potencia;
int   fanPWM      = 0;
bool  releLigado  = false;

// Histerese
const float T_H = 35.0, T_L = 34.0;   
const float P_H = 60, P_L = 59.0;   

void setup() {
  Serial.begin(115200); 
  pinMode(pinFan, OUTPUT);
  pinMode(pinRele, OUTPUT);

  digitalWrite(pinRele, RELAY_OFF);  

  setup_wifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int potValue = 0; 

  // Leitura do sensor TMP36
  float vTemp = analogRead(pinTemp) * 3.3 / 4095.0; 
  tempC = (vTemp - 0.5) * 100.0;

  // Leitura de tensão
  float vADC = analogRead(pinVolt) * 3.3 / 4095.0;
  tensao = vADC * (47.0 + 10.0) / 10.0;   // 

  // Leitura de corrente
  potValue = analogRead(potPin);
  corrente = map(potValue, 0, 4095, -500, 500) / 100.0;   

  potencia = tensao * corrente;

  // Controle da ventilação
  if (tempC >= T_H)      fanPWM = 255;
  else if (tempC <= T_L) fanPWM = 0;
  analogWrite(pinFan, fanPWM); 

  // Controle do relé 
  if (potencia <= P_H)      releLigado = true;
  else if (potencia > P_L) releLigado = false;
  digitalWrite(pinRele, releLigado ? RELAY_ON : RELAY_OFF);

  // Publicar via MQTT
  StaticJsonDocument<200> doc;
  doc["temperatura"] = tempC;
  doc["tensao"] = tensao;
  doc["corrente"] = corrente;
  doc["potencia"] = potencia;
  doc["fan_pwm"] = fanPWM;
  doc["rele_ligado"] = releLigado;

  char jsonBuffer[200];
  serializeJson(doc, jsonBuffer);

  client.publish(mqtt_topic, jsonBuffer);
  Serial.print("Publicado no tópico ");
  Serial.print(mqtt_topic);
  Serial.print(": ");
  Serial.println(jsonBuffer);

  delay(5000); // Publica a cada 5 segundos
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("conectado");
      
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

