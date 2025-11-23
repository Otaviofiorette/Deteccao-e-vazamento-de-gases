#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ThingSpeak.h>

// CONFIGURAÇÕES DE HARDWARE
#define RELE1 27
#define API_KEY "AIzaSyB9Qabb-TVml6J8iN7ZSyOIcwNuE3vx8Ww"
#define DATABASE_URL "https://vazamentodegasespi-default-rtdb.firebaseio.com/"

// WIFI
const char* ssid = "ISABEL";
const char* password = "ISABEL123";

// THINGSPEAK
unsigned long channelID = 3051952;
const char* writeAPIKey = "TR1SPYKQ9LCU4EBR";
WiFiClient client;

// PINOS
const int PINO_BUZZER = 23;
const int ledmq2 = 22;
const int ledmq135 = 21;
int mq2 = 36;   // VP
int mq135 = 39; // VN
int mq2limiar = 3000;
int mq135limiar = 3000;

// TEMPO DE ATUALIZAÇÃO
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 10000; // 10 segundos

// FIREBASE
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// NTP
const long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;
const char* ntpServer = "pool.ntp.org";

// CONTADOR DE LEITURAS
int leituraID = 0;

// MONITOR SERIAL
unsigned long lastSerialPrint = 0;
const unsigned long serialInterval = 5000;

void setup() {
  pinMode(RELE1, OUTPUT);
  digitalWrite(RELE1, HIGH);

  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(mq2, INPUT);
  pinMode(mq135, INPUT);
  pinMode(ledmq2, OUTPUT);
  pinMode(ledmq135, OUTPUT);

  digitalWrite(ledmq2, LOW);
  digitalWrite(ledmq135, LOW);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando-se ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi!");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sincronizando horário...");
  delay(2000);

  ThingSpeak.begin(client);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "otavio@teste.com", "otavio123")) {
    Serial.println("Firebase conectado com sucesso!");
  } else {
    Serial.printf("Erro ao conectar Firebase: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int analogmq135 = analogRead(mq135);
  int analogmq2 = analogRead(mq2);

  if (millis() - lastSerialPrint >= serialInterval) {
    lastSerialPrint = millis();
    Serial.println("Valor do gás (MQ135): " + String(analogmq135) + " | Valor do gás (MQ2): " + String(analogmq2));
  }

  bool gasMQ2 = analogmq2 > mq2limiar;
  bool gasMQ135 = analogmq135 > mq135limiar;
  bool alertaAtivo = gasMQ2 || gasMQ135;

  digitalWrite(ledmq2, gasMQ2 ? HIGH : LOW);
  digitalWrite(ledmq135, gasMQ135 ? HIGH : LOW);
  digitalWrite(RELE1, alertaAtivo ? HIGH : LOW);
  // digitalWrite(PINO_BUZZER, alertaAtivo ? HIGH : LOW);

  if ((millis() - lastUpdateTime >= updateInterval) || alertaAtivo) {
    lastUpdateTime = millis();

    ThingSpeak.setField(1, analogmq2);
    ThingSpeak.setField(2, analogmq135);
    ThingSpeak.writeFields(channelID, writeAPIKey);

    if (Firebase.ready()) {
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Erro ao obter hora!");
        return;
      }

      char isoTimestamp[30];
      strftime(isoTimestamp, sizeof(isoTimestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
      String dataHora = String(isoTimestamp);

      // MQ135
      FirebaseJson jsonMQ135;
      jsonMQ135.set("valor", analogmq135);
      jsonMQ135.set("alerta", gasMQ135 ? 1 : 0);
      jsonMQ135.set("sensor_id", 1);
      jsonMQ135.set("idLeitura", leituraID++);
      jsonMQ135.set("dataHora", dataHora);

      if (Firebase.RTDB.setJSON(&fbdo, "/leituras/MQ135", &jsonMQ135)) {
        Serial.println("MQ135 enviado com sucesso!");
      } else {
        Serial.printf("Erro ao enviar MQ135: %s\n", fbdo.errorReason().c_str());
      }

      // MQ2
      FirebaseJson jsonMQ2;
      jsonMQ2.set("valor", analogmq2);
      jsonMQ2.set("alerta", gasMQ2 ? 1 : 0);
      jsonMQ2.set("sensor_id", 2);
      jsonMQ2.set("idLeitura", leituraID++);
      jsonMQ2.set("dataHora", dataHora);

      if (Firebase.RTDB.setJSON(&fbdo, "/leituras/MQ2", &jsonMQ2)) {
        Serial.println("MQ2 enviado com sucesso!");
      } else {
        Serial.printf("Erro ao enviar MQ2: %s\n", fbdo.errorReason().c_str());
      }
    }
  }
}