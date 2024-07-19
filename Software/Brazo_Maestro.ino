//PLACA DOIT ESP32 DEVKIT V1
#include <WiFi.h>

const char* ssid = "NOMBRE DE LA RED"; //Colocar la red wifi
const char* password = "CONTRASEÑA"; //Colocar contraseña

const char* serverIP = "192.168.4.1"; //IP por defecto
const int serverPort = 80;

const int FLEX_THRESHOLD = 300;
const int FLEX_PINS[5] = {36, 39, 34, 35, 32};  // VP, VN, 34, 35, 32
const char FLEX_SIGNALS[5] = {'A', 'B', 'C', 'D', 'E'};
const char* FINGER_NAMES[5] = {"Pulgar", "Indice", "Medio", "Anular", "Menique"};

WiFiClient client;

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < 5; i++) {
    pinMode(FLEX_PINS[i], INPUT);
  }
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    Serial.println("Conectando al servidor...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Conectado al servidor");
    } else {
      Serial.println("Conexión fallida");
      delay(5000);
      return;
    }
  }

  for (int i = 0; i < 5; i++) {
    int sensorValue = analogRead(FLEX_PINS[i]);
    
    Serial.print(FINGER_NAMES[i]);
    Serial.print(": ");
    Serial.print(sensorValue);
    
    char angle;
    if (sensorValue > FLEX_THRESHOLD) {
      Serial.println(" - Enviando señal para 0 grados");
      angle = '0';
    } else {
      Serial.println(" - Enviando señal para 180 grados");
      angle = '1';
    }
    
    // Enviar datos al servidor
    client.write(FLEX_SIGNALS[i]);
    client.write(angle);
    
    delay(20); // Pequeña pausa entre lecturas
  }

  delay(1000); // Pausa antes del próximo ciclo de lecturas
}