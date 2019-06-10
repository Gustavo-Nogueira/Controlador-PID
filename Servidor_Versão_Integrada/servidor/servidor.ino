#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index_WebPage.h" 

WebServer server(80);

//Parâmetros de Desempenho
typedef struct{
  String tUp; //Tempo de Subida
  String tPeak; //Tempo de Pico
  String tAccommodation; //Tempo de Acomodação
  String erRegime; //Erro Regime
}PERFORMANCE_PARAMETERS;

//Variáveis de Controle
typedef struct{
  int Ki,Kp,Kd;  
}CONTROL_VARIABLES;

int PIN = 5; 
int PINStatus1 = 2;
int PINStatus2 = 4;
int distance;// Distância captada pelo sensor
int refSignal;// Andar desejado
String systemStatus;// Estabilizado,Estabilizando,Sem Estabilização.
CONTROL_VARIABLES ctrlVariables;
PERFORMANCE_PARAMETERS perfParameters;

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
  
  String jsonData = "{\"Distance\":\""+String(distance)+"\", \"Ki\":\""+ String(ctrlVariables.Ki) +"\", \"Kp\":\""+ String(ctrlVariables.Kp) +"\",\"Kd\":\""+ String(ctrlVariables.Kd) + "\",\"ReferenceSignal\":\""+ String(refSignal) +"\",\"SystemStatus\":\""+ String(systemStatus) +"\",\"UpTime\":\""+String(perfParameters.tUp)+"\",\"PeakTime\":\""+String(perfParameters.tPeak)+"\",\"AccommodationTime\":\""+String(perfParameters.tAccommodation)+"\",\"RegimeError\":\""+String(perfParameters.erRegime)+"\"}";
  
  server.send(200, "text/plane", jsonData); //Envia dados JSON para a requisição AJAX
}

void setup(void){ 
  pinMode(PIN, INPUT);//Entrada do sensor
  //Teste Status
  pinMode(PINStatus1, INPUT);
  pinMode(PINStatus2, INPUT);
  
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
   /*
       Valor   |     Status
        00     | Inicializando
        01     | Estabilizando
        10     | Estavel
        11     | Nao_Estavel
   */  
   
  if(!(digitalRead(PINStatus1))&&!(digitalRead(PINStatus2))){
    refSignal = 1;
    ctrlVariables.Kp = 10;
    ctrlVariables.Kd = 15;
    ctrlVariables.Ki = 20;
    systemStatus = "Inicializando";
  }
  else{
    if(!(digitalRead(PINStatus1))&&(digitalRead(PINStatus2))){
        distance = digitalRead(PIN);
        systemStatus = "Estabilizando";
    }
    else{
      if((digitalRead(PINStatus1))&&!(digitalRead(PINStatus2))){
        perfParameters.tUp = "10";
        perfParameters.tPeak = "20";
        perfParameters.tAccommodation = "30";
        perfParameters.erRegime = "40";
        systemStatus = "Estavel";
      }
      else{
        systemStatus = "Nao_Estavel";    
      }
    }
  }
 
  server.handleClient();
  delay(1);
}
