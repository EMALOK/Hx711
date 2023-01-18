#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
const char* ssid = "labTlc";
const char* password = "78945612311";

const char* serverName = "http://lezioni.alberghetti.it/5ATL/goni.t.091004/webservice/aggiornaPeso/";

#include <HX711.h>
#define calibration_factor 790.00 // calibrazione -> grammi
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
HX711 scale;

float weight_g = 0.0;

void get_data(){
  weight_g = scale.get_units();
}

void send_data(){

  WiFiClient client;
  HTTPClient http;
      
  http.begin(client, serverName);

  http.addHeader("Content-Type","application/x-www-form-urlencoded");

  String data = "id=3&peso=" + String(weight_g, 3);

  int httpResponseCode = http.POST(data);

  Serial.println(httpResponseCode);


}

void setup() {

  Serial.begin(9600);
  Serial.println("HX711 web connection");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  
}

void loop() {
  
  if(WiFi.status()== WL_CONNECTED){
    delay(500);

    get_data();

    send_data();
  }
}