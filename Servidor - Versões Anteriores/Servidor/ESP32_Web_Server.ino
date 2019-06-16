#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h> //https://github.com/bbx10/WebServer_tng

WebServer server ( 80 );
/*
  Utilizei o roteador do meu celular;
  Obs: não precisa ter internet =);
*/
const char* ssid     = "GustavoNr";
const char* password = "10101010";

int LEDPIN = 5;

String  ledState = "OFF";

void setup() 
{
  pinMode(LEDPIN, OUTPUT);
  
  Serial.begin(115200);

  connectToWifi();

  beginServer();
  
}

void loop() {
 
 server.handleClient();
 
 delay(1000);
 
}

/*
  connectToWifi():
  Conecta à rede passada pela string "ssid" com a senha "password", caso consiga conectar, imprime o valor aloca para ESP32 na rede indicada.

*/
void connectToWifi()
{
  WiFi.enableSTA(true);
  
  delay(2000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {//Caso não consiga estabelecer a conexão.
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
} 

/*
  beginServer():
  O código da função indica que quando uma requisição HTTP é recebida no caminho raiz (“/”), ela irá disparar a execução da função handleRoot;

*/
void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

/*
    handleRoot():
    Verifica se o navegador adicionou o parâmetro name = "LED";
*/
void handleRoot(){ 
  if ( server.hasArg("LED") ) {
    getSubmission();
  } else {
    server.send ( 200, "text/html", getPage() );//
  }  
}

/*
  getSubmission():
  Pega o valor selecionado no no seletor radio;
*/
void getSubmission() {
  String LEDValue;
  LEDValue = server.arg("LED");//Pegando valor do seletor on/off
  Serial.println("Set GPIO "); 
  Serial.print(LEDValue);
  
  if ( LEDValue == "1" ) {
    digitalWrite(LEDPIN, HIGH);
    ledState = "On";
    server.send ( 200, "text/html", getPage() );
  }
  else if( LEDValue == "0" ) 
  {
    digitalWrite(LEDPIN, LOW);
    ledState = "Off";
    server.send ( 200, "text/html", getPage() );
  } else 
  {
    Serial.println("Error Led Value");
  }
}

/*
  getPage():
  Retorna o conteúdo da página;
*/
String getPage(){
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  page += "<title>ESP32 WebServer</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>";
  page += "</head><body><h1>ESP32 WebServer</h1>";
  page += "<h3>LED</h3>";
  page += "<form action='/' method='POST'>";
  page += "<ul><li>LED";
  page += "<INPUT type='radio' name='LED' value='1'>ON";
  page += "<INPUT type='radio' name='LED' value='0'>OFF</li></ul>";
  page += "<INPUT type='submit' value='Submit'>";
  page += "</body></html>";
  
  //String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/><title>ESP32 WebServer</title><style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style></head><body><h1>ESP32 WebServer</h1><h3>LED</h3><form action='/' method='POST'><ul><li>LED<INPUT type='radio' name='LED' value='1'>ON<INPUT type='radio' name='LED' value='0'>OFF</li></ul><INPUT type='submit' value='Submit'></body></html>";

  
  return page;
}
