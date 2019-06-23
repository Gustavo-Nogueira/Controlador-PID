#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "Header_Functions.hpp"
#include "index_WebPage.h"

WebServer server(80);

VL53L0X sensor;

void handleRoot();
void initializeServer();
void handleUpdateRequest();

CONTROL_VARIABLES ctrlVariables;
ENCODER_VARIABLES encoderVariables;
PERFORMANCE_PARAMETERS perfParameters;
SYSTEM_CONFIGURATION_VARIABLES configVariables;
String systemStatus;// Estabilizado,Estabilizando,Sem Estabilização.

////////////////////////Temporarios de teste
int PINStatus1 = 2;
int PINStatus2 = 15;
int btn;
float distance;
////////////////////////

//SSID e Senha da Rede
const char* ssid     = "GustavoNr";
const char* password = "10101010";
//const char* ssid = "TORRE 01";
//const char* password = "nogueira@2019";

void setup() {
  Serial.begin(115200);
  
  pinMode(PINStatus1, INPUT);
  pinMode(PINStatus2, INPUT);
  
  initializeSensor(sensor);  
  initializePotentiometers(&encoderVariables);
  initializeButtons();
  initializeServer();
  
}

void loop() {
  
     /* Valor   |     Status
        01     | Estabilizando
        10     | Estavel
        11     | Nao_Estavel  */
        
  //Pegando entradas do sistema
  do{
    getControlVariables(&encoderVariables,&ctrlVariables);
    btn = readButtons();
    systemStatus = "Inicializando";
    server.handleClient();
  }while(!(btn));
  
  //Configurações de Refresh do sistema
  configVariables.totSamples = 0;
  //Converter andar pra mm -> refSignal
  
  configVariables.refSignal = btn;
  systemStatus = "Estabilizando";

  while(systemStatus == "Estabilizando"){
    if(!(digitalRead(PINStatus1))&&(digitalRead(PINStatus2))){//Estabilizando
      distance = filter((float) sensor.readRangeContinuousMillimeters());
      systemStatus = "Estabilizando";
    }
    else{
      if((digitalRead(PINStatus1))&&!(digitalRead(PINStatus2))){//Estável
          perfParameters.tUpDown = 10;
          perfParameters.tPeak = 20;
          perfParameters.overshoot = 30;
          perfParameters.tAccommodation = 40;
          perfParameters.erRegime = 50;
          systemStatus = "Estavel";
      }
      else{
        if((digitalRead(PINStatus1))&&(digitalRead(PINStatus2))){//Não Estável
          systemStatus = "Nao_Estavel";
        }
      }
    }
    server.handleClient();  
  }
  
  //Delay para atualizar a página
  for(int i = 0;i < 100;i++){
    server.handleClient();  
    delay(10);
  }
}

//Funções do Servidor
void initializeServer(){
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

  Serial.print("Conectando na rede ");
  Serial.println(ssid);

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

//Método de inicialização da página
void handleRoot(){
  String webPage = WEB_PAGE; //Carrega a página na string
  server.send(200, "text/html", webPage); //Enviar página da web
}

//Para retornar os dados à página solicitados pela requisição
void handleUpdateRequest(){
  
  String jsonData = "{\"Distance\":\""+String(distance)+"\", \"Ki\":\""+ String(ctrlVariables.ki) +"\", \"Kp\":\""+ String(ctrlVariables.kp) +"\",\"Kd\":\""+ String(ctrlVariables.kd) + "\",\"ReferenceSignal\":\""+ String((int)configVariables.refSignal) +"\",\"SystemStatus\":\""+ String(systemStatus) +"\",\"UpTime\":\""+String(perfParameters.tUpDown)+"\",\"PeakTime\":\""+String(perfParameters.tPeak)+"\",\"Overshoot\":\""+String(perfParameters.overshoot)+"\",\"AccommodationTime\":\""+String(perfParameters.tAccommodation)+"\",\"RegimeError\":\""+String(perfParameters.erRegime)+"\"}";

  server.send(200, "text/plane", jsonData); //Envia dados JSON para a requisição AJAX
}


