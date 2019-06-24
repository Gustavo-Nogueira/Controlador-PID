#include "server.h"
#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
 
const char* ssid = "anonimo";
const char* password =  "eugenio1";

AsyncWebServer server(80);

servidorSetup()
{
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
    request->send(SPIFFS, "/index_WebPage_HTML.html", "text/html");
  });
 /*
  server.on("/teste.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/teste.css", "text/css");
  });
*/

   server.begin();

}

servidorLoop()
{
    Serial.println("Qualquer coisa");
}