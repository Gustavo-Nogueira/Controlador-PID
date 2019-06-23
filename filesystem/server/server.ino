/*
1) Crie uma pasta data no mesmo diretório do código do servidor
2) Adicione o HTML e CSS na pasta data
3) Abra o arduino, vá em ferramentas e clique em Sketch Data Upload
4) Será informado uma mensagem de sucesso 
*/

#include "FS.h"
//#include "WiFi.h"
////#include "SPIFFS.h"
//#include "ESPAsyncWebServer.h"
//#include <ESP8266WebServer.h>
//#include <WiFiClient.h>
#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
 
const char* ssid = "anonimo";
const char* password =  "eugenio1";
 
AsyncWebServer server(80);
//ESP8266WebServer server(80);


void setup() {
  
  Serial.begin(115200);

  SPIFFS.begin();
 
  if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  //Para certificar, execute com cada arquivo criado:
  if(SPIFFS.exists("/teste.css"))
  {
    Serial.println("\n\nCSS exists!");
  }
  else Serial.println("\n\nNo File :(");

   WiFi.begin(ssid, password);
 
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
}
 
   Serial.println(WiFi.localIP());


    server.on("/teste", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/teste.html", "text/html");
  });
 
  server.on("/teste.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/teste.css", "text/css");
  });

   server.begin();

}
  

void loop() {

  ///Serial.println("Qualquer coisa");

}
