//MAESTRO
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const char* ssid = "NOMBRE DE LA RED"; //Colocar la red a la que te conectarás
const char* password = "CONTRASEÑA"; // Colocar la contraseña

WiFiServer server(80);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MEDIO 0
#define SERVO_ANULAR 1
#define SERVO_MENIQUE 2
#define SERVO_PULGAR 3
#define SERVO_INDICE 4

#define SERVOMIN  150
#define SERVOMAX  600

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }  // Espera hasta que el puerto serie esté listo
  
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(60);
  
  for (int i = 0; i <= 4; i++) {
    pwm.setPWM(i, 0, SERVOMIN);
  }
  
  Serial.println("Configurando Access Point...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dirección IP del AP (Esclavo): ");
  Serial.println(IP);
  
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Nuevo cliente conectado");
    while (client.connected()) {
      if (client.available() >= 2) {
        char finger = client.read();
        char angle = client.read();
        
        Serial.printf("Recibido: %c%c\n", finger, angle);
        
        switch(finger) {
          case 'A': moveServo(SERVO_PULGAR, angle, "Pulgar"); break;
          case 'B': moveServo(SERVO_INDICE, angle, "Indice"); break;
          case 'C': moveServo(SERVO_MEDIO, angle, "Medio"); break;
          case 'D': moveServo(SERVO_ANULAR, angle, "Anular"); break;
          case 'E': moveServo(SERVO_MENIQUE, angle, "Menique"); break;
          default: 
            Serial.println("Señal no reconocida");
        }
      }
    }
    client.stop();
    Serial.println("Cliente desconectado");
  }
}

void moveServo(int servoNum, char angle, const char* fingerName) {
  int pulseLen = (angle == '0') ? SERVOMIN : SERVOMAX;
  pwm.setPWM(servoNum, 0, pulseLen);
  Serial.printf("Moviendo servo %s a %s grados\n", fingerName, (angle == '0') ? "0" : "180");
}