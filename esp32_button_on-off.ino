#include <WiFi.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

#define DHTPIN 13 // Pino de conexão do sensor DHT22 (pino D13)
#define DHTTYPE DHT22 // Tipo do sensor DHT22

DHT dht22(DHTPIN, DHTTYPE); // Criação do objeto DHT para o sensor DHT22

String URL = "http://seu_scriptphp"; // URL do script PHP para envio dos dados

const char* ssid = "SSID da rede"; // Nome da rede Wi-Fi
const char* password = "Senha"; // Senha da rede Wi-Fi

int temperatura = 0; // Variável para armazenar a temperatura
int umidade = 0; // Variável para armazenar a umidade
int device = 1; // Identificador do dispositivo

bool sensorLigado = true; // Variável para controlar o estado do sensor (ligado/desligado)

AsyncWebServer server(80); // Criação do servidor web na porta 80

void setup() {
  Serial.begin(115200);

  dht22.begin(); // Inicializa o sensor DHT22

  connectWiFi(); // Conecta-se à rede Wi-Fi

  // Configura os handlers para as rotas do servidor web
  server.on("/ligar_sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    ligarSensor(); // Liga o sensor
    request->send(200, "text/plain", "Sensor ligado");
  });

  server.on("/desligar_sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    desligarSensor(); // Desliga o sensor
    request->send(200, "text/plain", "Sensor desligado");
  });

  server.begin(); // Inicia o servidor web
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi(); // Reconecta-se à rede Wi-Fi, se necessário
  }

  if (sensorLigado) {
    Load_DHT22_Data(); // Lê os dados do sensor DHT22
    String postData = "device=" + String(device) + "&temperatura=" + String(temperatura) + "&umidade=" + String(umidade);

    HTTPClient http;
    http.begin(URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST(postData); // Envia os dados para o script PHP
    String payload = http.getString();

    // Imprime informações de depuração
    Serial.print("URL : "); Serial.println(URL);
    Serial.print("Data: "); Serial.println(postData);
    Serial.print("httpCode: "); Serial.println(httpCode);
    Serial.print("payload : "); Serial.println(payload);
    Serial.println("--------------------------------------------------");
  } else {
    Serial.println("Sensor desligado");
  }

  delay(60000); // Aguarda 1 minuto
}

void Load_DHT22_Data() {
  temperatura = dht22.readTemperature(); // Lê a temperatura do sensor
  umidade = dht22.readHumidity(); // Lê a umidade do sensor

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Failed to read from DHT sensor!"); // Exibe mensagem de erro, se houver falha na leitura do sensor
    temperatura = 0;
    umidade = 0;
  }

  Serial.printf("Temperatura: %d °C\n", temperatura); // Exibe a temperatura no monitor serial
  Serial.printf("Umidade: %d %%\n", umidade); // Exibe a umidade no monitor serial
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password); // Conecta-se à rede Wi-Fi

  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

void ligarSensor() {
  sensorLigado = true; // Liga o sensor
  Serial.println("Sensor ligado");
}

void desligarSensor() {
  sensorLigado = false; // Desliga o sensor
  Serial.println("Sensor desligado");
}
