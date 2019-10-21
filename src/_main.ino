#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "Header_Functions.hpp"
#include "index_WebPage.h"
#include "PID.hpp"

WebServer server(80);

VL53L0X sensor;

void handleRoot();
void handleSubmit();
void initializeServer();
void handleUpdateRequest();

CONTROL_VARIABLES ctrlVariables;
ENCODER_VARIABLES encoderVariables;
PERFORMANCE_PARAMETERS perfParameters;
SYSTEM_CONFIGURATION_VARIABLES configVariables;
String systemStatus;// Estabilizado,Estabilizando,Sem Estabilização.

////////////////////////
float distance;
float output;
int selectedBtn;
bool checkStability;
bool btnStop;
float dataSamples[10000];
float lastDistance;
////////////////////////

//SSID e Senha da Rede
const char* ssid = "GustavoNr";
const char* password = "10101010";

void setup() {
  Serial.begin(115200);
  
  initializeSensor(sensor);  
  initializePotentiometers(&encoderVariables);
  initializeButtons();
  initializeServer();
  
  /////////////////////INICIALIZAÇÃO PWM/////////////////////////////////
  pinMode(2, OUTPUT);//Definimos o pino 2 (LED) como sa�da. 
  ledcAttachPin(2, 0);//Atribuimos o pino 2 ao canal 0.
  ledcSetup(0, 1000, 10);//Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.

}
PID instancePid(0.0,0.0,0.0);//Inicializandos obj. com os k's.
void loop() {

  ledcWrite(0, PWM_MAX - 205);//Escrevemos no canal 0, o duty cycle "i".   
  //Pegando entradas do sistema
  ctrlVariables.ki = 0.0;
  ctrlVariables.kp = 0.0;
  ctrlVariables.kd = 0.0;
  do{
    getControlVariables(&encoderVariables,&ctrlVariables);
    selectedBtn = readButtons();
    systemStatus = "Inicializando";
    server.handleClient();
  }while(!(selectedBtn));
  
  /*
  //Delay para atualizar a página
  for(int i = 0;i < 100;i++){
    server.handleClient();  
    delay(10);
  }*/
  
  //Configurações de Refresh do sistema
  instancePid.setKp(ctrlVariables.kp);
  instancePid.setKi(ctrlVariables.ki);
  instancePid.setKd(ctrlVariables.kd);
  configVariables.refSignal = distanceReference(selectedBtn);//Converter andar pra mm
  instancePid.setSetPoint(configVariables.refSignal);
  configVariables.totSamples = 0;
  configVariables.rateSamples = 0.10;
  checkStability = false;
  btnStop = false;
  instancePid.P = 0.0;
  instancePid.I = 0.0;
  instancePid.D = 0.0;


  //Reinicialização   
    float valor, referencia;
    float prevValue= 0;
    float episilon = 20;
    float faixa_top = filter((float) sensor.readRangeContinuousMillimeters()) + episilon ;    
    float faixa_back = filter((float) sensor.readRangeContinuousMillimeters()) - episilon ;    
    int consecutive = 0;
    int count = 0;
          
  while(!btnStop){
        ///////////////////////CONTROLE PID/////////////////////////
        distance = 712.0 -  filter((float) sensor.readRangeContinuousMillimeters());        
        Serial.println("---------------------------------------------");
        Serial.print("Distancia: ");
        Serial.println(distance);

        if(distance > 800.0) distance = lastDistance;
        if(distance < -15.0) distance = lastDistance; 
          
        
        server.handleClient();  
        instancePid.addNewSample(distance);
        //dataSamples[count] = distance;
        
        output = instancePid.process();
        
        Serial.print("P: ");
        Serial.println(instancePid.getProportionalAction());
        Serial.print("I: ");
        Serial.println(instancePid.getIntegralAction());
        Serial.print("D: ");
        Serial.println(instancePid.getDerivativeAction());
        
        Serial.print("PWM: ");
        
        //Filtragem dos RANGES
        if(output < PWM_RANGE_MIN){//20% 
          ledcWrite(0, PWM_MAX - PWM_RANGE_MIN);//Escrevemos no canal 0, o duty cycle "i".   
          Serial.print("OUTP: ");
          Serial.print(output);
          Serial.println(" | MIN: 205");
        }else{
            if(output > PWM_RANGE_MAX){//60%
              ledcWrite(0, PWM_MAX - PWM_RANGE_MAX);//Escrevemos no canal 0, o duty cycle "i".       
              Serial.print("OUTP: ");
              Serial.print(output);
              Serial.println(" | MAX: 614");
            }
            else{
              ledcWrite(0, PWM_MAX - output);//Escrevemos no canal 0, o duty cycle "i".   
              Serial.println(output);
            }
        }
        
        ///////////////////////////VERIFICAÇÃO DE ESTABILIDADE/////////////////////////////////
        //Não implementado, assim não foi possível extrair os parâmetros de desempenho.
        
        count++; 
        lastDistance = distance;
        systemStatus = "Estabilizando";
        server.handleClient();  
        delay(200);
  }

  if(checkStability){
    perfParameters = extractPerformanceParameter(configVariables,dataSamples);
    systemStatus = "Estavel";
  }
  else{
    systemStatus = "Nao_Estavel";
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
  Serial.print("Conectado na rede ");
  Serial.println(ssid);
  Serial.print("Endereco IP Designado: ");
  Serial.println(WiFi.localIP()); //IP designado para ESP pela rede

  server.on("/", handleRoot); // Para indicar a rotina que tratará o carregamento inicial da página
  server.on("/readRequest", handleUpdateRequest);// Para indicar a rotina que tratará as requisições da página
  server.on("/setSTOP", handleSubmit);// Submissão de parada
  
  server.begin();//Inícia o Servidor
  Serial.println("Servidor HTTP iniciado!");

}

//Trata submissão de parada
void handleSubmit() {
  String strRet = server.arg("STOPstate");
  if(strRet == "1"){
    instancePid.P = 0.0;
    instancePid.I = 0.0;
    instancePid.D = 0.0;
    btnStop = true;
  }
}

//Método de inicialização da página
void handleRoot(){
  String webPage = WEB_PAGE; //Carrega a página na string
  server.send(200, "text/html", webPage); //Enviar página da web
}

//Para retornar os dados à página solicitados pela requisição
void handleUpdateRequest(){

  String jsonData = "{\"Distance\":\""+String(distance)+"\", \"Ki\":\""+ String(ctrlVariables.ki) +"\", \"Kp\":\""+ String(ctrlVariables.kp) +"\",\"Kd\":\""+ String(ctrlVariables.kd) + "\",\"ReferenceSignal\":\""+ String(selectedBtn) +"\",\"SystemStatus\":\""+ String(systemStatus) +"\",\"UpTime\":\""+String(perfParameters.tUp)+"\",\"PeakTime\":\""+String(perfParameters.tPeak)+"\",\"Overshoot\":\""+String(perfParameters.overshoot)+"\",\"AccommodationTime\":\""+String(perfParameters.tAccommodation)+"\",\"RegimeError\":\""+String(perfParameters.erRegime)+"\"}";

  server.send(200, "text/plane", jsonData); //Envia dados JSON para a requisição AJAX
}

