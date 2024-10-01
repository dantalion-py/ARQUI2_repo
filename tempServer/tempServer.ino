#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>

const int oneWireBus1 = 13;
const int oneWireBus2 = 26;

OneWire oneWire1(oneWireBus1);
OneWire oneWire2(oneWireBus2);

// Instancias de DallasTemperature para cada sensor
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);

const char* ssid = "saitama123";        
const char* password = "12345678"; 

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(3000);

  // Iniciar los sensores
  sensors1.begin();
  sensors2.begin();

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
    sensors1.requestTemperatures();
    float temperatureC_1 = sensors1.getTempCByIndex(0);

    sensors2.requestTemperatures();
    float temperatureC_2 = sensors2.getTempCByIndex(0);

    // Si la solicitud es para los datos de temperatura
    if (request.indexOf("/temperature") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();

      // Respuesta JSON con las dos temperaturas
      client.print("{\"temperature_1\":");
      client.print(temperatureC_1, 1); // Temperatura del sensor 1 con 2 decimales
      client.print(", \"temperature_2\":");
      client.print(temperatureC_2, 1); // Temperatura del sensor 2 con 2 decimales
      client.println("}");
    }

    delay(1);
    client.stop();
  }
}
