#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>

const int oneWireBus = 27;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

const char* ssid = "1234";        
const char* password = "12345678"; 

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(3000);
  sensors.begin();
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");
  server.begin();
  Serial.println(WiFi.localIP());

}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Solicitar temperaturas
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);

    // Si la solicitud es para los datos
    if (request.indexOf("/temperature") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.print("{\"temperature\":");
      client.print(temperatureC, 1);
      client.println("}");
    }

    delay(1);
    client.stop();
  }
}
