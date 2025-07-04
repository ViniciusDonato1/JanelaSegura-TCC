#include <WiFi.h>
#include <PubSubClient.h>

const char* nomewifi = "TCCjanela";
const char* senhawifi = "tcc12345";

const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_comando = "janela/comando";
const char* mqtt_status = "janela/status"; 


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);       
  Serial2.begin(9600);   

  conectarWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(recebeComando);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    conectarWiFi();
  }


  if (!client.connected()) {
    reconectarMQTT();
  }

  client.loop();


  if (Serial2.available()) {
    String status = Serial2.readStringUntil('\n');
    status.trim();

    if (status == "ABERTO" || status == "FECHADO") {
      Serial.print("Enviando status para MQTT: ");
      Serial.println(status);
      client.publish(mqtt_status, status.c_str());
    }
  }

  delay(10);
}


void conectarWiFi() {
  Serial.print("Conectando-se a ");
  Serial.println(nomewifi);
  WiFi.begin(nomewifi, senhawifi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}


void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      client.subscribe(mqtt_comando);
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 2 segundos...");
      delay(2000);
    }
  }
}

void recebeComando(char* topic, byte* payload, unsigned int length) {
  String comando = "";
  for (unsigned int i = 0; i < length; i++) {
    comando += (char)payload[i];
  }
  comando.trim();

  Serial.print("Comando recebido: ");
  Serial.println(comando);

  if (comando == "abrir" || comando == "fechar") {
    Serial2.println(comando); 
    Serial.println("Comando enviado ao Arduino via Serial2");
  } else {
    Serial.println("Comando inválido ignorado.");
  }
}

