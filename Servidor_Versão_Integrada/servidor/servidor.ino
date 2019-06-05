#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index_WebPage.h" 

WebServer server(80);
int PIN = 5; 

int vtest = 0;
int Ki,Kp,Kd;//Variáveis de Controle
int distance;// Distância captada pelo sensor
int refSignal;// Andar desejado
int systemStatus;// Estabilizado,Estabilizando,Sem Estabilização.

//SSID e Senha da Rede
//const char* ssid = "TORRE 01";
//const char* password = "nogueira@2019";

const char* ssid     = "GustavoNr";
const char* password = "10101010";

//Método de inicialização da página  
void handleRoot() { 
  String webPage = WEB_PAGE; //Carrega a página na string
  server.send(200, "text/html", webPage); //Enviar página da web
}

//Para retornar os dados à página solicitados pela requisição 
void handleUpdateRequest(){
  Ki = ++vtest;
  Kp = ++vtest;
  Kd = ++vtest;
  refSignal = ++vtest;
  systemStatus = ++vtest;
  
  int distance = digitalRead(PIN);
  
  String jsonData = "{\"Distance\":\""+String(distance)+"\", \"Ki\":\""+ String(Ki) +"\", \"Kp\":\""+ String(Kp) +"\",\"Kd\":\""+ String(Kd) + "\",\"ReferenceSignal\":\""+ String(refSignal) +"\",\"SystemStatus\":\""+ String(systemStatus) +"\"}";
  
  server.send(200, "text/plane", jsonData); //Envia dados JSON para a requisição AJAX
}

 
void setup(void){
  pinMode(PIN, INPUT);//Entrada do sensor
  Serial.begin(115200);
  Serial.println();
  Serial.println("Inicializando...");
 
  /*
   //Define a ESP como ponto de Acesso
    WiFi.mode(WIFI_AP); 
    WiFi.softAP(ssid, password);
  */

  //Conecta a ESP em uma rede
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  Serial.println("Conectando na rede ");
  Serial.print(ssid);
 
  //Aguardando estabeler conexão na rede
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //Monitor Serial: caso consiga estabelecer a conexão
  Serial.println("");
  Serial.print("Conectado na rede ");
  Serial.println(ssid);
  Serial.print("Endereco IP Designado: ");
  Serial.println(WiFi.localIP()); //IP designado para ESP pela rede
 
  server.on("/", handleRoot); // Para indicar a rotina que tratará o carregamento inicial da página
  server.on("/readRequest", handleUpdateRequest);// Para indicar a rotina que tratará as requisições da página
  
  server.begin();//Inícia o Servidor
  Serial.println("Servidor HTTP iniciado!");
}

//Tratando as requisições do cliente
void loop(void){
  server.handleClient();
  delay(1);
}
