#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Configurações do WiFi
const char* ssid = "ICOMP2 WIFI";  // Nome da rede Wi-Fi
const char* password = "";         // Senha da rede Wi-Fi (em branco para o simulador)
const char* host = "http://api.thingspeak.com"; // Host para a conexão com a API do ThingSpeak
const char* api_key = "R1KE8CIX9FF0PPRN"; // Chave da API do ThingSpeak

// Configurações do sensor DHT
#define DHTPIN 4     // Pino digital ao qual o DHT11 está conectado
#define DHTTYPE DHT11  // Define o tipo do sensor DHT como DHT11
DHT dht(DHTPIN, DHTTYPE);  // Criação do objeto DHT

void setup() {
  Serial.begin(115200);   // Inicia a comunicação serial
  dht.begin();            // Inicializa o sensor DHT11
  WiFi.begin(ssid, password); // Conecta à rede Wi-Fi

  // Aguarda a conexão com o Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando-se ao WiFi...");
  }
  
  Serial.println("Conectado ao WiFi");
}

void loop() {
  // Lê a umidade e a temperatura
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verifica se alguma leitura falhou e sai cedo (para tentar novamente).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler do sensor DHT11!");
  } else {
    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.print("%  Temperatura: ");
    Serial.print(temperature);
    Serial.println("°C ");

    // Constrói a URL da solicitação HTTP
    String url = String(host) + "/update?api_key=" + api_key + "&field1=" + String(temperature) + "&field2=" + String(humidity);
    Serial.println("URL: " + url);
    
    // Inicializa o cliente HTTP
    HTTPClient http;
    http.begin(url);
    
    // Envia a solicitação HTTP GET e imprime o código de resposta HTTP
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("HTTP Request failed, error: ");
      Serial.println(httpResponseCode);
    }
    
    // Finaliza a conexão HTTP
    http.end();
  }
  delay(1000);  // Aguarda 1 segundos antes de enviar novamente
}