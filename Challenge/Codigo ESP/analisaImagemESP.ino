#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
 
String WIFI_SSID = "Galaxy";
String WIFI_PASS = "12345678";
 
int RelePin = 5;
 
String API_BASE_URL = "https://trina-unprolific-pseudoservilely.ngrok-free.dev";
 
String BEARER_TOKEN = "ac6000862b0c890c626683d0f6b77f641a799dfccd28112267f911a3eb62f4c60";
 
int estadoDoPrograma = 0;
 
void recebeAnaliseData(String dataUsuario);
void recebeAnalisePorId(String idAnalise);
 

 
void setup() {
  Serial.begin(115200);
  delay(1000);
 
  pinMode(RelePin, OUTPUT);
 
  digitalWrite(RelePin, LOW); 
 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("\nSetup concluído. Insira uma imagem de uma placa solar.");
}
 
void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim(); 
 
    if (input.length() == 0) {
      return;
    }
 
    if (estadoDoPrograma == 0) {
      Serial.println("Analisando a imagem: " + input);
      envia_analise(input);
      delay (7000);
     
      Serial.println("\n--- ANÁLISE SOLICITADA ---");
      Serial.println("Digite o ID da análise para buscar o resultado:");
      estadoDoPrograma = 1; 
     
    } else if (estadoDoPrograma == 1) {
      Serial.println("Buscando dados para a análise com o id: " + input);

      recebeAnalisePorId(input);
      
      Serial.println("\n--- BUSCA CONCLUÍDA ---");
      Serial.println("Insira uma nova imagem de placa solar para começar de novo:");
      estadoDoPrograma = 0;
    }
  }
}
 
 
void envia_analise(String imagem)  {
 
  String url = API_BASE_URL + "/challenge/analisar";
  StaticJsonDocument<128> doc_payload;
 
  doc_payload["url"] = imagem;
 
  String payload;
  serializeJson(doc_payload, payload);
 
  HTTPClient http;
 
  Serial.print("Enviando POST para: ");
  Serial.println(url);
  Serial.print("Payload: ");
  Serial.println(payload);
 
  http.begin(url); 
 
  http.addHeader("Authorization", "Bearer " + BEARER_TOKEN);
  http.addHeader("User-Agent", "insomnia/11.6.1");
  http.addHeader("Content-Type", "application/json");
 
  int httpCode = http.POST(payload);
 
  Serial.printf("[HTTP] Código de resposta: %d\n", httpCode);
 
  delay(5000);
  if (httpCode > 0) {
    String responsePayload = http.getString();
    Serial.println("Payload recebido:");
 
    DynamicJsonDocument doc_resposta(2048);
    DeserializationError error = deserializeJson(doc_resposta, responsePayload);
    if (error) {
      Serial.print("Falha ao fazer parse do JSON da resposta: ");
      Serial.println(error.c_str());
      Serial.println("Resposta bruta:");
      Serial.println(responsePayload); 
    } else {

      serializeJsonPretty(doc_resposta, Serial);
      Serial.println();
    }
  } else {
    Serial.printf("[HTTP] Falha no POST, erro: %s\n", http.errorToString(httpCode).c_str());
  }
 
  http.end();
}
 
 
 
 
void recebeAnaliseData(String dataUsuario) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Erro: Wi-Fi desconectado.");
    return;
  }
 
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client->setInsecure();
  }
 
  HTTPClient http;
 
  String url = String(API_BASE_URL) + "/challenge/analises/data/" + dataUsuario;
  Serial.println("URL da requisição: " + url);
 
  if (http.begin(*client, url)) {
    http.setUserAgent("insomnia/11.6.1");
   
    String authHeader = "Bearer " + BEARER_TOKEN;
    http.addHeader("Authorization", authHeader);
 
    int httpCode = http.GET();
 
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.printf("[HTTP] Código de resposta: %d\n", httpCode);
      Serial.println("Payload recebido:");
      Serial.println(payload);
     
      if (httpCode == HTTP_CODE_OK) {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) {
          const char* analiseResp = doc[0]["saida"]["analise"];
          Serial.println("--- ANÁLISE EXTRAÍDA ---");
          Serial.println(analiseResp);
          Serial.println("------------------------");
        } else {
          Serial.println("Falha ao analisar o JSON.");
        }
      }
 
    } else {
      Serial.printf("[HTTP] Falha no GET, código de erro: %d\n", httpCode);
      Serial.printf("[HTTP] Mensagem: %s\n", http.errorToString(httpCode).c_str());
    }
   
    http.end();
  } else {
    Serial.printf("[HTTP] Não foi possível iniciar a conexão para: %s\n", url.c_str());
  }
 
  delete client;
}
void recebeAnalisePorId(String idAnalise) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Erro: Wi-Fi desconectado.");
    return;
  }
 
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client->setInsecure();
  }
 
  HTTPClient http;
 
  String url = String(API_BASE_URL) + "/challenge/analises/" + idAnalise;
  Serial.println("URL da requisição: " + url);
 
  if (http.begin(*client, url)) {
   
    http.setUserAgent("insomnia/11.6.1");
   
    String authHeader = "Bearer " + BEARER_TOKEN;
    http.addHeader("Authorization", authHeader);
 
    int httpCode = http.GET();
 
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.printf("[HTTP] Código de resposta: %d\n", httpCode);
      Serial.println("Payload recebido:");
      Serial.println(payload);
     
      if (httpCode == HTTP_CODE_OK) {
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) {
          String analiseResp = doc["saida"]["needCleaning"];
          Serial.println("--- ANÁLISE EXTRAÍDA ---");
          Serial.println(analiseResp);
         
          if(analiseResp == "true"){
            Serial.println("DECISÃO: Ligando o relé.");
            digitalWrite(RelePin, HIGH); 
          }else{
            Serial.println("DECISÃO: Mantendo o relé desligado.");
            digitalWrite(RelePin, LOW); 
          }
          Serial.println("------------------------");
        } else {
          Serial.println("Falha ao analisar o JSON.");
        }
      }
 
    } else {
      Serial.printf("[HTTP] Falha no GET, código de erro: %d\n", httpCode);
      Serial.printf("[HTTP] Mensagem: %s\n", http.errorToString(httpCode).c_str());
    }
   
    http.end();
  } else {
    Serial.printf("[HTTP] Não foi possível iniciar a conexão para: %s\n", url.c_str());
  }
 
  delete client;
}
 